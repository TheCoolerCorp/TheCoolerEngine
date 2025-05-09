#include  "Core/GraphicsAPI/Vulkan/VulkanObjectDescriptor.h"

#include "Core/Assertion/Assertion.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "GamePlay/ComponentsBase/Material.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			/* Create the descriptor without data at creation, need to use SetUniform or SetTextures to copy data into the uniform and UpdateUniform functions to update the descriptor
			* Type represent the type of descriptor (common to all like a camera or per object like model matrix)
			* SetCounnt represent the number of set (for example frame in flight for object that need constant update)
			* UniformCount represent the number of set for uniform 
			* SubuniformCount represent the number of buffer for each set of uniform (for example 3 for the uniform matrix and only 1 for materials data) 
			* Tyes represent ALL the types of data you wish to send to this descriptor, int in enum represent the int in vulkan, can easily add types
			*/
			void VulkanObjectDescriptor::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IGraphicPipeline* a_graphicPipeline, RHI::DescriptorSetTarget a_type, uint32_t a_setCount, uint32_t a_uniformCount, std::vector<uint32_t> a_subUniformCount, std::vector<RHI::DescriptorSetDataType> a_types)
			{
				m_type = a_type;

				// Pipeline target
				if (a_graphicPipeline->GetType() == RHI::PipelineType::Lit)
				{
					m_pipelineType = RHI::LitDescriptor;
				}
				else if (a_graphicPipeline->GetType() == RHI::PipelineType::Other)
				{
					m_pipelineType = RHI::SkyBoxDescriptor;
				}

				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				// Choose the right layout (shader set, example : layout(set = 0, binding = ...)
				const VkDescriptorSetLayout t_layout = ChooseLayout(a_graphicPipeline->CastVulkan()->GetSetLayouts(), a_type);

				std::vector<VkDescriptorType> t_types = std::vector<VkDescriptorType>(a_types.size());
				for (int i = 0; i < a_types.size(); ++i)
				{
					t_types[i] = static_cast<VkDescriptorType>(static_cast<int>(a_types[i]));
				}
				CreatePool(t_device,a_setCount, t_types);
				t_types.clear();

				// Create the descriptors and buffers
				CreateDescriptorSets(t_device, t_layout, a_setCount);
				CreateBuffers(a_uniformCount, a_subUniformCount);
			}

			void VulkanObjectDescriptor::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				// Create vulkan object from rhi object.
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

				m_sets.clear();

				vkDeviceWaitIdle(t_logicalDevice);
				vkDestroyDescriptorPool(t_logicalDevice, m_pool, nullptr);
				
				for (int i = 0; i < m_uniforms.size(); ++i)
				{
					for (int j = 0; j < m_uniforms[i].size(); ++j)
					{
						m_uniforms[i][j]->Destroy(a_logicalDevice);
					}
				}
				DestroyBuffers();
			}

			/* Set the texture
			* Destination binding l : (layout(set = ..., binding = 0)
			* Count represent the number of descriptor
			*/
			void VulkanObjectDescriptor::SetTexture(RHI::ILogicalDevice* a_logicalDevice, RHI::IImage* a_image, uint32_t a_dstBinding, uint32_t a_count)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				vkDeviceWaitIdle(t_device);

				VkDescriptorImageInfo t_textureInfo{};
				t_textureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				t_textureInfo.imageView = a_image->CastVulkan()->GetView();
				t_textureInfo.sampler = a_image->CastVulkan()->GetSampler();

				for (int i = 0; i < m_sets.size(); ++i)
				{
					VkWriteDescriptorSet texture;
					texture.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					texture.pNext = nullptr;
					texture.dstSet = m_sets[i];
					texture.dstBinding = a_dstBinding;
					texture.dstArrayElement = 0;
					texture.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					texture.descriptorCount = a_count;
					texture.pImageInfo = &t_textureInfo;

					vkUpdateDescriptorSets(t_device, 1, &texture, 0, nullptr);
				}
			}

			/* Set the uniform
			* BufferIndex : which set to sent those data
			* Destination binding l : (layout(set = ..., binding = 0)
			* DescriptorCount represent the number of descriptor
			*/
			void VulkanObjectDescriptor::SetUniform(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, uint32_t a_bufferIndex, void* a_data, uint32_t a_dataSize, uint32_t a_dstBinding, uint32_t a_descriptorCount)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				RHI::BufferData t_bufferData;
				t_bufferData.mUboData = a_data;
				t_bufferData.mUboSize = a_dataSize;

				for (int j = 0; j < m_uniforms[a_bufferIndex].size(); ++j)
				{
					m_uniforms[a_bufferIndex][j]->Create(RHI::BufferType::UBO, t_bufferData, a_physicalDevice, a_logicalDevice, a_commandPool);
				}


				ASSERT(m_uniforms[a_bufferIndex].size() == m_sets.size() || m_uniforms[a_bufferIndex].size() == 1, "Cannot match uniform size to DescriptorSet size");
				for (int i = 0; i < m_sets.size(); ++i)
				{
					VkDescriptorBufferInfo t_bufferInfo{};

					if (m_uniforms[a_bufferIndex].size() != m_sets.size())
					{
						t_bufferInfo.buffer = m_uniforms[a_bufferIndex][0]->GetBuffer();
					}
					else
					{
						t_bufferInfo.buffer = m_uniforms[a_bufferIndex][i]->GetBuffer();
					}
					t_bufferInfo.offset = 0;
					t_bufferInfo.range = a_dataSize;
					
					VkWriteDescriptorSet uniform;
					uniform.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					uniform.pNext = nullptr;
					uniform.dstSet = m_sets[i];
					uniform.dstBinding = a_dstBinding;
					uniform.dstArrayElement = 0;
					uniform.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					uniform.descriptorCount = a_descriptorCount;
					uniform.pBufferInfo = &t_bufferInfo;

					vkUpdateDescriptorSets(t_device, 1, &uniform, 0, nullptr);
				}
			}

			/* Update the uniform buffer
			* BufferIndex : which set to sent those data
			* ImageIndex : which uniform in the set to update (for example depending on the currently rendering frame index from swapchain)
			*/
			void VulkanObjectDescriptor::UpdateUniforms(RHI::ILogicalDevice* a_logicalDevice, uint32_t a_bufferIndex,void* a_data, uint32_t a_dataSize, int a_imageIndex)
			{
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

				void* t_data;
				vkMapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_bufferIndex][a_imageIndex]->GetMemory(), 0, a_dataSize, 0, &t_data);
				memcpy(t_data, a_data, a_dataSize);
				vkUnmapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_bufferIndex][a_imageIndex]->GetMemory());
			}

			VkDescriptorSetLayout VulkanObjectDescriptor::ChooseLayout(std::vector<VulkanSetLayout> a_layouts, RHI::DescriptorSetTarget a_type)
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

			void VulkanObjectDescriptor::CreatePool(VkDevice a_logicalDevice, uint32_t a_count , std::vector<VkDescriptorType> a_types)
			{
				std::unordered_map<VkDescriptorType, uint32_t> t_types;
				for (size_t i = 0; i < a_types.size(); ++i)
				{
					t_types[a_types[i]] += 1;
				}

				std::vector<VkDescriptorPoolSize> t_poolSizes;
				for (const auto& [type, count] : t_types)
				{
					VkDescriptorPoolSize t_poolSize{};
					t_poolSize.type = type;
					t_poolSize.descriptorCount = count * a_count;
					t_poolSizes.push_back(t_poolSize);
				}

				VkDescriptorPoolCreateInfo t_poolInfo{};
				t_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				t_poolInfo.poolSizeCount = static_cast<uint32_t>(t_poolSizes.size());
				t_poolInfo.pPoolSizes = t_poolSizes.data();
				t_poolInfo.maxSets = a_count;

				VK_CHECK(vkCreateDescriptorPool(a_logicalDevice, &t_poolInfo, nullptr, &m_pool), "failed to create descriptorPool");
			}

			void VulkanObjectDescriptor::CreateDescriptorSets(VkDevice a_logicalDevice, VkDescriptorSetLayout a_descriptorSetLayout, uint32_t a_setCount)
			{
				m_sets.resize(a_setCount);

				std::vector<VkDescriptorSetLayout> layouts(a_setCount, a_descriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = m_pool;
				allocInfo.descriptorSetCount = a_setCount;
				allocInfo.pSetLayouts = layouts.data();

				VK_CHECK(vkAllocateDescriptorSets(a_logicalDevice, &allocInfo, m_sets.data()), "Can't allocate descriptor sets");
			}

			void VulkanObjectDescriptor::CreateBuffers(uint32_t a_uniformCount, std::vector<uint32_t> a_subUniformCount)
			{
				if (a_uniformCount != a_subUniformCount.size())
				{
					LOG_ERROR("Wrong size of buffer list size for uniform specification");
				}

				m_uniforms.resize(a_uniformCount);
				for (int i = 0; i < m_uniforms.size(); ++i)
				{
					m_uniforms[i].resize(a_subUniformCount[i]);
					for (int j = 0; j < m_uniforms[i].size(); ++j)
					{
						m_uniforms[i][j] = new VulkanBuffer;
					}
				}
			}

			void VulkanObjectDescriptor::DestroyBuffers()
			{
				for (auto& t_uniform : m_uniforms)
				{
					for (auto& t_subBuffer : t_uniform)
					{
						delete t_subBuffer;
					}
				}
			}


		}
	}
}
