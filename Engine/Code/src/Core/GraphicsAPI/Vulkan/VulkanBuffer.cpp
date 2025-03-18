#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"

#include "Core/Assertion/Assertion.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanBuffer::Create(const RHI::BufferType a_type, const RHI::BufferData a_data,
			                          RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool)
			{
				ASSERT(a_type != RHI::BufferType::NONE, "buffer of type NONE !");

				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();
				const VkPhysicalDevice t_physicalDevice = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();

				VkDeviceSize t_bufferSize{};

				if (a_type == RHI::BufferType::VERTEX)
				{
					t_bufferSize = sizeof(a_data.mVertices[0]) * static_cast<uint32_t>(a_data.mVertices.size());
				}
				else if (a_type == RHI::BufferType::INDEX)
				{
					t_bufferSize = sizeof(a_data.mIndices[0]) * static_cast<uint32_t>(a_data.mIndices.size());
				}
				else if (a_type == RHI::BufferType::UBO)
				{
					t_bufferSize = a_data.mUboSize;
				}

				if (a_type == RHI::BufferType::UBO)
				{
					ASSERT(a_data.mUboData != nullptr, "UBO data is null !");

					CreateBuffer(t_bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_buffer,
					             m_bufferMemory, t_device, t_physicalDevice);

					void* t_data;
					vkMapMemory(t_device, m_bufferMemory, 0, t_bufferSize, 0, &t_data);
					memcpy(t_data, a_data.mUboData, t_bufferSize);
					vkUnmapMemory(t_device, m_bufferMemory);
				}
				else
				{
					VkBuffer t_stagingBuffer;
					VkDeviceMemory t_stagingBufferMemory;
					CreateBuffer(t_bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, t_stagingBuffer, t_stagingBufferMemory, t_device, t_physicalDevice);

					void* t_data;
					vkMapMemory(t_device, t_stagingBufferMemory, 0, t_bufferSize, 0, &t_data);
					if (a_type == RHI::BufferType::VERTEX)
					{
						memcpy(t_data, a_data.mVertices.data(), t_bufferSize);
					}
					else if (a_type == RHI::BufferType::INDEX)
					{
						memcpy(t_data, a_data.mIndices.data(), t_bufferSize);
					}
					vkUnmapMemory(t_device, t_stagingBufferMemory);

					if (a_type == RHI::BufferType::VERTEX)
					{
						CreateBuffer(t_bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
						             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory, t_device,
						             t_physicalDevice);
					}
					else if (a_type == RHI::BufferType::INDEX)
					{
						CreateBuffer(t_bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
						             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory, t_device,
						             t_physicalDevice);
					}

					CopyBuffer(a_commandPool->CastVulkan(), t_stagingBuffer, m_buffer, t_bufferSize, a_logicalDevice->CastVulkan());

					vkDestroyBuffer(t_device, t_stagingBuffer, nullptr);
					vkFreeMemory(t_device, t_stagingBufferMemory, nullptr);
				}
			}

			void VulkanBuffer::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				vkDestroyBuffer(t_device, m_buffer, nullptr);
				vkFreeMemory(t_device, m_bufferMemory, nullptr);
			}

			void VulkanBuffer::CreateBuffer(const VkDeviceSize a_size, const VkBufferUsageFlags a_usage,
			                                const VkMemoryPropertyFlags a_properties, VkBuffer& a_buffer, VkDeviceMemory& a_bufferMemory, const VkDevice a_device, const VkPhysicalDevice a_physicalDevice)
			{
				VkBufferCreateInfo t_bufferInfo{};
				t_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				t_bufferInfo.size = a_size;
				t_bufferInfo.usage = a_usage;
				t_bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(a_device, &t_bufferInfo, nullptr, &a_buffer) != VK_SUCCESS) {
					throw std::runtime_error("failed to create buffer!");
				}

				VkMemoryRequirements t_memRequirements;
				vkGetBufferMemoryRequirements(a_device, a_buffer, &t_memRequirements);

				VkMemoryAllocateInfo t_allocInfo{};
				t_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				t_allocInfo.allocationSize = t_memRequirements.size;
				t_allocInfo.memoryTypeIndex = FindMemoryType(t_memRequirements.memoryTypeBits, a_properties, a_physicalDevice);

				if (vkAllocateMemory(a_device, &t_allocInfo, nullptr, &a_bufferMemory) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate buffer memory!");
				}

				vkBindBufferMemory(a_device, a_buffer, a_bufferMemory, 0);
			}

			void VulkanBuffer::CopyBuffer(const VulkanCommandPool* a_commandPool, const VkBuffer a_srcBuffer, const VkBuffer a_dstBuffer,
				const VkDeviceSize a_size, const VulkanLogicalDevice* a_logicalDevice)
			{
				const VkDevice t_device = a_logicalDevice->GetVkDevice();
				const VkQueue t_queue = a_logicalDevice->GetGraphicsQueue();
				const VkCommandPool t_commandPool = a_commandPool->GetVkCommandPool();

				const VkCommandBuffer t_commandBuffer = VulkanCommandPool::BeginSingleTimeCommands(t_device, t_commandPool);

				VkBufferCopy t_copyRegion{};
				t_copyRegion.size = a_size;
				vkCmdCopyBuffer(t_commandBuffer, a_srcBuffer, a_dstBuffer, 1, &t_copyRegion);

				VulkanCommandPool::EndSingleTimeCommands(t_commandBuffer, t_commandPool, t_device, t_queue);
			}

			uint32_t VulkanBuffer::FindMemoryType(const uint32_t a_typeFilter, const VkMemoryPropertyFlags a_properties,
			                                      const VkPhysicalDevice a_physicalDevice)
			{
				VkPhysicalDeviceMemoryProperties t_memProperties;
				vkGetPhysicalDeviceMemoryProperties(a_physicalDevice, &t_memProperties);

				for (uint32_t i = 0; i < t_memProperties.memoryTypeCount; i++) {
					if ((a_typeFilter & (1 << i)) && (t_memProperties.memoryTypes[i].propertyFlags & a_properties) == a_properties) {
						return i;
					}
				}

				LOG_CRITICAL("Failed to find suitable memory type!");
				return 0;
			}
		}
	}
}
