#include  "Core/GraphicsAPI/Vulkan/VulkanRenderObject.h"

#include  "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{

			void VulkanRenderObject::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IGraphicPipeline* a_graphicPipeline, RHI::DescriptorSetTarget a_type, int a_count, std::vector<RHI::DescriptorSetType> a_types)
			{
				// Type : Common to all object in shader or per object.
				m_type = a_type;

				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				// Choose the right layout
				const VkDescriptorSetLayout t_layout = ChooseLayout(a_graphicPipeline->CastVulkan()->GetSetLayouts(), a_type);


				// Create the pool
				std::vector<VkDescriptorType> t_types = std::vector<VkDescriptorType>(a_types.size());
				for (int i = 0; i < t_types.size(); ++i)
				{
					t_types[i] = static_cast<VkDescriptorType>(static_cast<int>(a_types[i]));
				}
				CreatePool(t_device, a_count, t_types);
				t_types.clear();

				// Create the descriptors
				CreateDescriptorSets(t_device, t_layout, a_count);
				CreateBuffers(a_count);
			}

			void VulkanRenderObject::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				// Create vulkan object from rhi object.
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

				vkFreeDescriptorSets(t_logicalDevice, m_pool, static_cast<uint32_t>(m_sets.size()), m_sets.data());
				m_sets.clear();

				vkDestroyDescriptorPool(t_logicalDevice, m_pool, nullptr);

				for (int i = 0; i < m_uniforms.size(); ++i)
				{
					m_uniforms[i]->Destroy(a_logicalDevice);
				}
				DestroyBuffers();

			}

			void VulkanRenderObject::SetTexture(RHI::ILogicalDevice* a_logicalDevice, RHI::IImage* a_image, uint32_t a_dstBinding, uint32_t a_count)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				for (int i = 0; i < m_sets.size(); ++i)
				{
					VkDescriptorImageInfo t_textureInfo{};
					t_textureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					t_textureInfo.imageView = a_image->CastVulkan()->GetView();
					t_textureInfo.sampler = a_image->CastVulkan()->GetSampler();

					VkWriteDescriptorSet texture;
					texture.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					texture.dstSet = m_sets[i];
					texture.dstBinding = a_dstBinding;
					texture.dstArrayElement = 0;
					texture.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					texture.descriptorCount = a_count;
					texture.pImageInfo = &t_textureInfo;

					vkUpdateDescriptorSets(t_device, 1, &texture, 0, nullptr);
				}
			}

			ENGINE_API void VulkanRenderObject::SetMat(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, void* a_matData, uint32_t a_dstBinding, uint32_t a_count)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				for (int i = 0; i < m_sets.size(); ++i)
				{
					RHI::BufferData t_matInfo;
					t_matInfo.mUboData = a_matData;
					t_matInfo.mUboSize = 16 * sizeof(float);
					m_uniforms[i]->Create(RHI::BufferType::UBO, t_matInfo, a_physicalDevice, a_logicalDevice, a_commandPool);

					VkDescriptorBufferInfo t_bufferInfo{};
					t_bufferInfo.buffer = m_uniforms[i]->GetBuffer();
					t_bufferInfo.offset = 0;
					t_bufferInfo.range = 16 * sizeof(float);

					VkWriteDescriptorSet mat;
					mat.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					mat.dstSet = m_sets[i];
					mat.dstBinding = a_dstBinding;
					mat.dstArrayElement = 0;
					mat.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					mat.descriptorCount = a_count;
					mat.pBufferInfo = &t_bufferInfo;

					vkUpdateDescriptorSets(t_device, 1, &mat, 0, nullptr);
				}
				
			}

			void VulkanRenderObject::UpdateUniforms(RHI::ILogicalDevice* a_logicalDevice, void* a_data, int a_imageIndex)
			{
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

				void* t_data;
				vkMapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_imageIndex]->GetMemory(), 0, sizeof(VulkanBuffer), 0, &t_data);
				memcpy(t_data, a_data, 16 * sizeof(float));
				vkUnmapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_imageIndex]->GetMemory());
			}

			VkDescriptorSetLayout VulkanRenderObject::ChooseLayout(std::vector<VulkanSetLayout> a_layouts, RHI::DescriptorSetTarget a_type)
			{
				for (int i = 0; i < a_layouts.size(); ++i)
				{
					if (static_cast<int>(a_layouts[i].mType) == static_cast<int>(a_type))
					{
						return a_layouts[i].mLayout;
					}
				}
				return VK_NULL_HANDLE;
			}

			void VulkanRenderObject::CreatePool(VkDevice a_logicalDevice, uint32_t a_count, std::vector<VkDescriptorType> a_types)
			{
				std::vector<VkDescriptorPoolSize> t_poolSizes = std::vector<VkDescriptorPoolSize>(a_types.size());

				for (int i = 0; i < t_poolSizes.size(); ++i)
				{
					VkDescriptorPoolSize t_poolSize = {};
					t_poolSize.type = a_types[i];
					t_poolSize.descriptorCount = a_count;
					t_poolSizes[i] = t_poolSize;
				}
				VkDescriptorPoolCreateInfo t_poolInfo{};
				t_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				t_poolInfo.poolSizeCount = static_cast<uint32_t>(t_poolSizes.size());
				t_poolInfo.pPoolSizes = t_poolSizes.data();
				t_poolInfo.maxSets = a_count;

				VK_CHECK(vkCreateDescriptorPool(a_logicalDevice, &t_poolInfo, nullptr, &m_pool), "failed to create descriptorPool");
			}

			void VulkanRenderObject::CreateDescriptorSets(VkDevice a_logicalDevice, VkDescriptorSetLayout a_descriptorSetLayout, uint32_t a_count)
			{
				std::vector<VkDescriptorSetLayout> layouts(a_count, a_descriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = m_pool;
				allocInfo.descriptorSetCount = a_count;
				allocInfo.pSetLayouts = layouts.data();

				m_sets.resize(a_count);

				VK_CHECK(vkAllocateDescriptorSets(a_logicalDevice, &allocInfo, m_sets.data()), "Can't allocate descriptor sets");
			}

			void VulkanRenderObject::CreateBuffers(uint32_t a_count)
			{
				m_uniforms.resize(a_count);
				for (auto& t_uniform : m_uniforms)
				{
					t_uniform = new VulkanBuffer;
				}
			}

			void VulkanRenderObject::DestroyBuffers()
			{
				for (auto& t_uniform : m_uniforms)
				{
					delete t_uniform;
				}
			}


		}
	}
}