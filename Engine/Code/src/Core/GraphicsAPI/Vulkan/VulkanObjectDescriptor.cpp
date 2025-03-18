#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescritptor.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include "GamePlay/GameObject.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			ENGINE_API void VulkanObjectDescriptor::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool, GamePlay::GameObject* a_gameObject, int a_size)
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
				VK_CHECK(vkAllocateDescriptorSets(t_logicalDevice, &allocInfo, m_descriptorSets.data()), "Can't allocate descriptor sets");

                for (size_t i = 0; i < a_size; i++)
                {
                   /* VkDescriptorBufferInfo bufferInfo{};
                    bufferInfo.buffer = m_uniforms[i]->GetBuffer();
                    bufferInfo.offset = 0;
                    bufferInfo.range = sizeof(VulkanBuffer);*/

                   /* VkDescriptorImageInfo imageInfo{};
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = vkTexture->imageView;
                    imageInfo.sampler = vkSampler->textureSampler;

                    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[0].dstSet = descriptorSets[i];
                    descriptorWrites[0].dstBinding = 0;
                    descriptorWrites[0].dstArrayElement = 0;
                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptorWrites[0].descriptorCount = 1;
                    descriptorWrites[0].pBufferInfo = &bufferInfo;

                    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[1].dstSet = descriptorSets[i];
                    descriptorWrites[1].dstBinding = 1;
                    descriptorWrites[1].dstArrayElement = 0;
                    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descriptorWrites[1].descriptorCount = 1;
                    descriptorWrites[1].pImageInfo = &imageInfo;

                    vkUpdateDescriptorSets(vkDevice->device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);*/
                }
			}

			ENGINE_API void VulkanObjectDescriptor::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{

			}
		}
	}
}