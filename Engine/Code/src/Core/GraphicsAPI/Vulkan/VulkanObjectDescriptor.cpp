#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescritptor.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "GamePlay/GameObject.h"
#include "GamePlay/TextureComponent.h"
#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanObjectDescriptor::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool, RHI::ICommandPool* a_commandPool, GamePlay::GameObject* a_gameObject, int a_size)
			{
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkDescriptorSetLayout t_descriptorSetLayout = a_pipeline->CastVulkan()->GetObjectDescriptorSetLayout();
				VkDescriptorPool t_descriptorPool = a_descriptorPool->CastVulkan()->GetPool();

				std::vector<VkDescriptorSetLayout> layouts(a_size, t_descriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = t_descriptorPool;
				allocInfo.descriptorSetCount = static_cast<uint32_t>(a_size);
				allocInfo.pSetLayouts = layouts.data();

				m_descriptorSets.resize(a_size);
				m_uniforms.resize(a_size);

				VK_CHECK(vkAllocateDescriptorSets(t_logicalDevice, &allocInfo, m_descriptorSets.data()), "Can't allocate descriptor sets");

                for (size_t i = 0; i < a_size; i++)
                {
                    m_uniforms[i] = new VulkanBuffer;

                    Math::mat4 t_mat = a_gameObject->GetTransform().GetModel();

                    RHI::BufferData t_uniformData;
                    t_uniformData.mUboData = t_mat.mElements.data();
                    t_uniformData.mUboSize = 16 * sizeof(float);
                    m_uniforms[i]->Create(RHI::BufferType::UBO, t_uniformData, a_physicalDevice, a_logicalDevice, a_commandPool);

                    VkDescriptorBufferInfo t_bufferInfo{};
                    t_bufferInfo.buffer = m_uniforms[i]->GetBuffer();
                    t_bufferInfo.offset = 0;
                    t_bufferInfo.range = 16 * sizeof(float);

                	VkDescriptorImageInfo t_imageInfo{};
                    t_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					t_imageInfo.imageView = 0;/*a_gameObject->GetComponent<GamePlay::TextureComponent>()->GetTexture()->GetImage()->CastVulkan()->GetView();*/
					t_imageInfo.sampler = 0;/*a_gameObject->GetComponent<GamePlay::TextureComponent>()->GetTexture()->GetImage()->CastVulkan()->GetSampler();*/

                    std::array<VkWriteDescriptorSet, 2> t_descriptorWrites{};

                    t_descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    t_descriptorWrites[0].dstSet = m_descriptorSets[i];
                    t_descriptorWrites[0].dstBinding = 0;
                    t_descriptorWrites[0].dstArrayElement = 0;
                    t_descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    t_descriptorWrites[0].descriptorCount = 1;
                    t_descriptorWrites[0].pBufferInfo = &t_bufferInfo;

					t_descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					t_descriptorWrites[1].dstSet = m_descriptorSets[i];
					t_descriptorWrites[1].dstBinding = 1;
					t_descriptorWrites[1].dstArrayElement = 0;
					t_descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					t_descriptorWrites[1].descriptorCount = 1;
					t_descriptorWrites[1].pImageInfo = &t_imageInfo;

                    vkUpdateDescriptorSets(t_logicalDevice, static_cast<uint32_t>(t_descriptorWrites.size()), t_descriptorWrites.data(), 0, nullptr);
                }
			}

			void  VulkanObjectDescriptor::Update(const uint32_t a_frameIndex, RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData)
			{
                void* t_data;
                vkMapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_frameIndex]->GetMemory(), 0, sizeof(VulkanBuffer), 0, &t_data);
                memcpy(t_data, a_uploadData, sizeof(a_uploadData));
				vkUnmapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_frameIndex]->GetMemory());
			}

			void VulkanObjectDescriptor::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				for (int i = 0; i < m_uniforms.size(); ++i)
				{
					m_uniforms[i]->Destroy(a_logicalDevice);
				}
				m_uniforms.clear();
				m_descriptorSets.clear();
			}
		}
	}
}