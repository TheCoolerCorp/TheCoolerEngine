#include  "Core/GraphicsAPI/Vulkan/VulkanRenderObject.h"

#include  "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanRenderObject::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ICommandPool* a_commandPool, RHI::IGraphicPipeline* a_graphicPipeline, int a_maxFrame)
			{
				// Create vulkan object from rhi object.
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkDescriptorSetLayout t_descriptorSetLayout = a_graphicPipeline->CastVulkan()->GetObjectDescriptorSetLayout();

				// Create pool
				CreatePool(t_logicalDevice, a_maxFrame);

				// Create descriptorSets.
				CreateDescriptorSets(t_logicalDevice, t_descriptorSetLayout, a_maxFrame);

				// Create empty uniform buffers.
				m_uniforms.resize(a_maxFrame);

				for (int i = 0; i < m_uniforms.size(); ++i)
				{
					m_uniforms[i] = new VulkanBuffer;
				}
			}

			void VulkanRenderObject::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				// Create vulkan object from rhi object.
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

				//vkFreeDescriptorSets(t_logicalDevice, m_pool, static_cast<uint32_t>(m_sets.size()), m_sets.data());
				m_sets.clear();

				vkDestroyDescriptorPool(t_logicalDevice, m_pool, nullptr);

				for (int i = 0; i < m_uniforms.size(); ++i)
				{
					m_uniforms[i]->Destroy(a_logicalDevice);
					delete m_uniforms[i];
				}
			}

			void VulkanRenderObject::CreatePool(VkDevice a_logicalDevice, int a_maxFrame)
			{
				std::array<VkDescriptorPoolSize, 2> t_poolSizes{};
				t_poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				t_poolSizes[0].descriptorCount = static_cast<uint32_t>(a_maxFrame);
				t_poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				t_poolSizes[1].descriptorCount = static_cast<uint32_t>(a_maxFrame);

				VkDescriptorPoolCreateInfo t_poolInfo{};
				t_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				t_poolInfo.poolSizeCount = static_cast<uint32_t>(t_poolSizes.size());
				t_poolInfo.pPoolSizes = t_poolSizes.data();
				t_poolInfo.maxSets = static_cast<uint32_t>(a_maxFrame);

				VK_CHECK(vkCreateDescriptorPool(a_logicalDevice, &t_poolInfo, nullptr, &m_pool), "failed to create descriptorPool");
			}

			void VulkanRenderObject::CreateDescriptorSets(VkDevice a_logicalDevice, VkDescriptorSetLayout a_descriptorSetLayout, int a_maxFrame)
			{
				std::vector<VkDescriptorSetLayout> layouts(a_maxFrame, a_descriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = m_pool;
				allocInfo.descriptorSetCount = static_cast<uint32_t>(a_maxFrame);
				allocInfo.pSetLayouts = layouts.data();

				m_sets.resize(a_maxFrame);

				VK_CHECK(vkAllocateDescriptorSets(a_logicalDevice, &allocInfo, m_sets.data()), "Can't allocate descriptor sets");
			}

			void VulkanRenderObject::SetData(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, RHI::IImage* a_image, void* a_data, int a_maxFrame)
			{
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkPhysicalDevice t_physicalDevice = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();
				VkCommandPool t_commandPool = a_commandPool->CastVulkan()->GetVkCommandPool();

				for (size_t i = 0; i < a_maxFrame; i++)
				{
					RHI::BufferData t_uniformData;
					t_uniformData.mUboData = a_data;
					t_uniformData.mUboSize = 16 * sizeof(float);
					m_uniforms[i]->Create(RHI::BufferType::UBO, t_uniformData, a_physicalDevice, a_logicalDevice, a_commandPool);

					VkDescriptorBufferInfo t_bufferInfo{};
					t_bufferInfo.buffer = m_uniforms[i]->GetBuffer();
					t_bufferInfo.offset = 0;
					t_bufferInfo.range = 16 * sizeof(float);


					VkDescriptorImageInfo t_imageInfo{};
					t_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					t_imageInfo.imageView = a_image->CastVulkan()->GetView();
					t_imageInfo.sampler = a_image->CastVulkan()->GetSampler();


					std::array<VkWriteDescriptorSet, 2> t_descriptorWrites{};

					t_descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					t_descriptorWrites[0].dstSet = m_sets[i];
					t_descriptorWrites[0].dstBinding = 0;
					t_descriptorWrites[0].dstArrayElement = 0;
					t_descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					t_descriptorWrites[0].descriptorCount = 1;
					t_descriptorWrites[0].pBufferInfo = &t_bufferInfo;
									   
					t_descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					t_descriptorWrites[1].dstSet = m_sets[i];
					t_descriptorWrites[1].dstBinding = 1;
					t_descriptorWrites[1].dstArrayElement = 0;
					t_descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					t_descriptorWrites[1].descriptorCount = 1;
					t_descriptorWrites[1].pImageInfo = &t_imageInfo;
						
					vkUpdateDescriptorSets(t_logicalDevice, static_cast<uint32_t>(t_descriptorWrites.size()), t_descriptorWrites.data(), 0, nullptr);
				}
			}

			//void VulkanRenderObject::SetTexture(RHI::ILogicalDevice* a_logicalDevice, RHI::IImage* a_image, int a_maxFrame)
			//{
			//	/*VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

			//	for (size_t i = 0; i < a_maxFrame; i++)
			//	{
			//		VkDescriptorImageInfo t_imageInfo{};
			//		t_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			//		t_imageInfo.imageView = a_image->CastVulkan()->GetView();
			//		t_imageInfo.sampler = a_image->CastVulkan()->GetSampler();

			//		VkWriteDescriptorSet m_ImageDescriptor{};

			//		m_ImageDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			//		m_ImageDescriptor.dstSet = m_sets[i];
			//		m_ImageDescriptor.dstBinding = 1;
			//		m_ImageDescriptor.dstArrayElement = 0;
			//		m_ImageDescriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			//		m_ImageDescriptor.descriptorCount = 1;
			//		m_ImageDescriptor.pImageInfo = &t_imageInfo;

			//		vkUpdateDescriptorSets(t_logicalDevice, static_cast<uint32_t>(m_sets.size()), &m_ImageDescriptor, 0, nullptr);
			//	}*/
			//}

			void VulkanRenderObject::UpdateUniforms(RHI::ILogicalDevice* a_logicalDevice, void* a_data, int a_imageIndex)
			{
				// Create vulkan object from rhi object.
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

				void* t_data;
				vkMapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_imageIndex]->GetMemory(), 0, sizeof(VulkanBuffer), 0, &t_data);
				memcpy(t_data, a_data, sizeof(a_data));
				vkUnmapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_imageIndex]->GetMemory());
			}
		}
	}
}