#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescritptor.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "GamePlay/GameObject.h"
#include "GamePlay/TextureComponent.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanObjectDescriptor::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool, RHI::ICommandPool* a_commandPool, GamePlay::GameObject* a_gameObject, int a_size)
			{
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkDescriptorSetLayout t_descriptorSetLayout = a_pipeline->CastVulkan()->GetDescriptorSetLayout();
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

                    VkDescriptorBufferInfo bufferInfo{};
                    bufferInfo.buffer = m_uniforms[i]->GetBuffer();
                    bufferInfo.offset = 0;
                    bufferInfo.range = 16 * sizeof(float);

                	VkDescriptorImageInfo imageInfo{};
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = a_gameObject->GetComponent<GamePlay::TextureComponent>()->GetTexture()->GetImage()->CastVulkan()->GetView();
					imageInfo.sampler = a_gameObject->GetComponent<GamePlay::TextureComponent>()->GetTexture()->GetImage()->CastVulkan()->GetSampler();

                    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[0].dstSet = m_descriptorSets[i];
                    descriptorWrites[0].dstBinding = 0;
                    descriptorWrites[0].dstArrayElement = 0;
                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptorWrites[0].descriptorCount = 1;
                    descriptorWrites[0].pBufferInfo = &bufferInfo;

					descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[1].dstSet = m_descriptorSets[i];
					descriptorWrites[1].dstBinding = 1;
					descriptorWrites[1].dstArrayElement = 0;
					descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorWrites[1].descriptorCount = 1;
					descriptorWrites[1].pImageInfo = &imageInfo;

                    vkUpdateDescriptorSets(t_logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
                }
			}

			void  VulkanObjectDescriptor::Update(uint32_t a_frameIndex, RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData)
			{
                void* data;
                vkMapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_frameIndex]->GetMemory(), 0, sizeof(VulkanBuffer), 0, &data);
                memcpy(data, a_uploadData, sizeof(a_uploadData));
				vkUnmapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_frameIndex]->GetMemory());
				
			}

			void VulkanObjectDescriptor::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				for (int i = 0; i < m_uniforms.size(); ++i)
				{
					m_uniforms[i]->Destroy(a_logicalDevice);
				}
				m_descriptorSets.clear();
			}
		}
	}
}