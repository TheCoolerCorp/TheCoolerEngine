#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanBuffer::CreateVertexBuffer(std::vector<Ressources::Vertex> a_vertices) {}

			void VulkanBuffer::CreateIndexBuffer(std::vector<uint32_t> a_indices) {}

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
