#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"

#include "Core/Assertion/Assertion.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/Interfaces/ILogicalDevice.h"
#include "Core/Interfaces/IRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanShader.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescriptor.h"
#include "Ressources/Vertex.h"
#include "Core/Logger/Logger.h"
namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			/*
			* Automaticely read shader data extract using spir-v reflect to create the shader and set the layout(s) and all binding
			* Only the shader part is abstract, didn't have time to abstract all the rest (rasterization mode, etc...)
			*/
			void VulkanGraphicPipeline::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IRenderPass* a_renderPass, RHI::PipelineType a_type, std::array<RHI::IShader*, 2> a_vertFragShaders, std::vector<RHI::IShader*> a_additionalShaders)
			{
				m_type = a_type;

				#pragma region Shader
				std::vector<VkPipelineShaderStageCreateInfo> shaderStages = std::vector<VkPipelineShaderStageCreateInfo>(2 + a_additionalShaders.size());

				// Vertex and fragment shaders
				for (int i = 0; i < 2; ++i)
				{
					VkPipelineShaderStageCreateInfo shaderStageInfo{};
					shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					shaderStageInfo.stage = a_vertFragShaders[i]->CastVulkan()->GetFlags();
					shaderStageInfo.module = a_vertFragShaders[i]->CastVulkan()->GetHandle();
					shaderStageInfo.pName = "main";

					shaderStages[i] = shaderStageInfo;
				}

				// Additional shaders like tesselation or compute shaders (not implemented for now)
				/*for (int i = 0; i < a_additionalShaders.size(); ++i)
				{
					VkPipelineShaderStageCreateInfo shaderStageInfo{};
					shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					shaderStageInfo.stage = a_vertFragShaders[i]->CastVulkan()->GetFlags();
					shaderStageInfo.module = a_vertFragShaders[i]->CastVulkan()->GetHandle();
					shaderStageInfo.pName = "main";

					shaderStages[2 + i] = shaderStageInfo;
				}*/
				#pragma endregion

				#pragma region VertexInput
				VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
				vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				auto bindingDescription = Resource::VulkanVertexSpec::getBindingDescription();
				auto attributeDescriptions = Resource::VulkanVertexSpec::getAttributeDescriptions();
				vertexInputInfo.vertexBindingDescriptionCount = 1;
				vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
				vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
				vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
				#pragma endregion

				#pragma region Spec
				VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
				inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				inputAssembly.primitiveRestartEnable = VK_FALSE;

				VkPipelineViewportStateCreateInfo viewportState{};
				viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				viewportState.viewportCount = 1;
				viewportState.scissorCount = 1;

				VkPipelineRasterizationStateCreateInfo rasterizer{};
				rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rasterizer.depthClampEnable = VK_FALSE;
				rasterizer.rasterizerDiscardEnable = VK_FALSE;
				rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
				rasterizer.lineWidth = 1.0f;
				rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
				// SWITCH HERE TO MAKE THE -X face appear
				rasterizer.cullMode = VK_CULL_MODE_NONE;
				//rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
				rasterizer.depthBiasEnable = VK_FALSE;
				#pragma endregion

				// Multisampling disable
				VkPipelineMultisampleStateCreateInfo multisampling{};
				multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				multisampling.sampleShadingEnable = VK_FALSE;
				multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

				// Depth activate by default, need to be modular later
				VkPipelineDepthStencilStateCreateInfo depthStencil{};
				depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				depthStencil.depthTestEnable = VK_TRUE;
				depthStencil.depthWriteEnable = VK_TRUE;
				depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
				depthStencil.depthBoundsTestEnable = VK_FALSE;
				depthStencil.stencilTestEnable = VK_FALSE;

				VkPipelineColorBlendAttachmentState colorBlendAttachment{};
				colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
				colorBlendAttachment.blendEnable = VK_FALSE; // Set to true if uncomment below
				//colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
				//colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				//colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
				//colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
				//colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				//colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

				VkPipelineColorBlendStateCreateInfo colorBlending{};
				colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				colorBlending.logicOpEnable = VK_FALSE;
				colorBlending.logicOp = VK_LOGIC_OP_COPY;
				colorBlending.attachmentCount = 1;
				colorBlending.pAttachments = &colorBlendAttachment;

				std::vector<VkDynamicState> dynamicStates =
				{
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
				};

				VkPipelineDynamicStateCreateInfo dynamicState {};
				dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
				dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
				dynamicState.pDynamicStates = dynamicStates.data();


				// Get Unique set in shaders (Vert and frag only for now)
				std::vector<uint32_t> recordedSets = std::vector<uint32_t>(0);
				for (int i = 0; i < 2; ++i)
				{
					for (const auto& set : a_vertFragShaders[i]->CastVulkan()->GetSets())
					{
						if (std::find(recordedSets.begin(), recordedSets.end(), set.mIndex) == recordedSets.end())
						{
							recordedSets.push_back(set.mIndex);
						}
					}
				}

				m_setslayouts.resize(recordedSets.size());
				for (int i = 0; i < recordedSets.size(); ++i)
				{
					uint32_t setIndex = i;
					m_setslayouts[i].mIndex = i;
					std::vector<VkDescriptorSetLayoutBinding> setBindings = std::vector<VkDescriptorSetLayoutBinding>(0);

					for (int j = 0; j < 2; ++j)
					{
						for (auto& set : a_vertFragShaders[j]->CastVulkan()->GetSets())
						{
							if (set.mIndex == setIndex)
							{
								for (int k = 0; k < set.mBindings.size(); ++k)
								{
									m_setslayouts[i].mType = GetType(set.mBindings[k].mName);
									VkDescriptorSetLayoutBinding setBinding{};
									setBinding.binding = set.mBindings[k].mIndex;
									setBinding.descriptorType = set.mBindings[k].mType;
									setBinding.descriptorCount = set.mBindings[k].mCount;
									setBinding.stageFlags = set.mBindings[k].mFlags;
									setBinding.pImmutableSamplers = nullptr;

									setBindings.push_back(setBinding);
								}
							}
						}
					}
					VkDescriptorSetLayoutCreateInfo SetCreateInfo{};
					SetCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
					SetCreateInfo.bindingCount = static_cast<uint32_t>(setBindings.size());
					SetCreateInfo.pBindings = setBindings.data();

					VK_CHECK(vkCreateDescriptorSetLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), &SetCreateInfo, nullptr, &m_setslayouts[i].mLayout), "Failed to create descriptor set layout");
				}
				

				std::vector<VkDescriptorSetLayout> descriptorSetLayouts = std::vector<VkDescriptorSetLayout>(m_setslayouts.size());
				for (int i = 0; i < m_setslayouts.size(); ++i)
				{
					descriptorSetLayouts[i] = m_setslayouts[i].mLayout;
				}

				VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
				pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

				VK_CHECK(vkCreatePipelineLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), &pipelineLayoutInfo, nullptr, &m_layout),
					"Failed to create pipeline layout");

				VkGraphicsPipelineCreateInfo pipelineInfo{};
				pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				pipelineInfo.stageCount = 2;
				pipelineInfo.pStages = shaderStages.data();
				pipelineInfo.pVertexInputState = &vertexInputInfo;
				pipelineInfo.pInputAssemblyState = &inputAssembly;
				pipelineInfo.pViewportState = &viewportState;
				pipelineInfo.pRasterizationState = &rasterizer;
				pipelineInfo.pMultisampleState = &multisampling;
				pipelineInfo.pColorBlendState = &colorBlending;
				pipelineInfo.pDynamicState = &dynamicState;
				pipelineInfo.layout = m_layout;
				pipelineInfo.renderPass = a_renderPass->CastVulkan()->GetSceneRenderPass()->GetRenderPass();
				pipelineInfo.subpass = 0;
				pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
				pipelineInfo.basePipelineIndex = -1;
				pipelineInfo.pDepthStencilState = &depthStencil;

				VK_CHECK(vkCreateGraphicsPipelines(a_logicalDevice->CastVulkan()->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline), "Failed to create graphic pipeline");

				a_renderPass->CastVulkan()->GetSceneRenderPass()->AddPipelineDependency(this);
			}

			void VulkanGraphicPipeline::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				for (int i = 0; i < m_setslayouts.size(); ++i)
				{
					vkDestroyDescriptorSetLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), m_setslayouts[i].mLayout, nullptr);
				}
				vkDestroyPipeline(a_logicalDevice->CastVulkan()->GetVkDevice(), m_pipeline, nullptr);
				vkDestroyPipelineLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), m_layout, nullptr);
			}

			void VulkanGraphicPipeline::Bind(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, RHI::ISwapChain* a_swapChain)
			{
				const VulkanCommandPool* t_commandPool = a_commandPool->CastVulkan();

				if (!t_commandPool->mCommandBuffers[a_commandBufferIndex].empty())
				{
					const VkCommandBuffer t_commandBuffer = t_commandPool->mCommandBuffers[a_commandBufferIndex][a_swapChain->GetCurrentFrame()];
					vkCmdBindPipeline(t_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
				}
			}

			void VulkanGraphicPipeline::BindObjects(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, uint32_t a_currentFrame, uint32_t  a_imageIndex, std::vector<RHI::IBuffer*> a_indexBuffers,
													std::vector<RHI::IBuffer*> a_vertexBuffers, std::vector<uint32_t> a_indicesCount, std::vector<RHI::IObjectDescriptor*> a_objectsDescriptors)
			{
				const VulkanCommandPool* t_commandPool = a_commandPool->CastVulkan();

				if (t_commandPool->mCommandBuffers[a_commandBufferIndex].empty())
				{
					return;
				}

				const VkCommandBuffer t_commandBuffer = t_commandPool->mCommandBuffers[a_commandBufferIndex][a_currentFrame];
				const VkDeviceSize t_offsets[] = { 0 };

				for (int i = 0; i < a_objectsDescriptors.size(); ++i)
				{
					if (!a_vertexBuffers.at(i))
					{
						continue;
					}
					if (a_vertexBuffers.at(i)->CastVulkan()->GetBuffer() != nullptr)
					{
						VkBuffer t_vertexBuffer = a_vertexBuffers.at(i)->CastVulkan()->GetBuffer();

						vkCmdBindVertexBuffers(t_commandBuffer, 0, 1, &t_vertexBuffer, t_offsets);
					}

					if (a_indexBuffers.at(i)->CastVulkan()->GetBuffer() != nullptr)
					{
						VkBuffer t_indexBuffer = a_indexBuffers.at(i)->CastVulkan()->GetBuffer();

						vkCmdBindIndexBuffer(t_commandBuffer, t_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
					}


					VulkanObjectDescriptor* t_objectDescriptor = a_objectsDescriptors.at(i)->CastVulkan();

					uint32_t t_descriptorIndex = a_imageIndex;
					if (t_objectDescriptor->GetDescriptorSets().size() != t_commandPool->mCommandBuffers[a_commandBufferIndex].size() && !t_objectDescriptor->GetDescriptorSets().empty())
					{
						t_descriptorIndex = static_cast<uint32_t>(t_objectDescriptor->GetDescriptorSets().size()) - 1;
					}
					vkCmdBindDescriptorSets(t_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_layout, (int)t_objectDescriptor->GetType(), 1, &t_objectDescriptor->GetDescriptorSets()[t_descriptorIndex], 0, nullptr);

					if (a_indicesCount.at(i))
					{
						vkCmdDrawIndexed(t_commandBuffer, a_indicesCount.at(i), 1, 0, 0, 0);
					}
				}

			}

			void VulkanGraphicPipeline::BindSingleDescriptors(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, uint32_t a_currentFrame, uint32_t  a_imageIndex, std::vector<RHI::IObjectDescriptor*> a_objectsDescriptors)
			{
				const VulkanCommandPool* t_commandPool = a_commandPool->CastVulkan();

				if (t_commandPool->mCommandBuffers[a_commandBufferIndex].empty())
				{
					return;
				}

				const VkCommandBuffer t_commandBuffer = t_commandPool->mCommandBuffers[a_commandBufferIndex][a_currentFrame];

				for (int i = 0; i < a_objectsDescriptors.size(); ++i)
				{
					VulkanObjectDescriptor* t_objectDescriptor = a_objectsDescriptors.at(i)->CastVulkan();

					uint32_t t_descriptorIndex = a_imageIndex;
					if (t_objectDescriptor->GetDescriptorSets().size() != t_commandPool->mCommandBuffers[a_commandBufferIndex].size() && !t_objectDescriptor->GetDescriptorSets().empty())
					{
						t_descriptorIndex = static_cast<uint32_t>(t_objectDescriptor->GetDescriptorSets().size()) - 1;
					}
					vkCmdBindDescriptorSets(t_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_layout, (int)t_objectDescriptor->GetType(), 1, &t_objectDescriptor->GetDescriptorSets()[t_descriptorIndex], 0, nullptr);
				}
			}

			SetLayoutType VulkanGraphicPipeline::GetType(std::string a_string)
			{
				if (a_string.find(COMMON) != std::string::npos)
				{
					return Common;
				}
				else if (a_string.find(PER) != std::string::npos)
				{
					return Per;
				}
				else if (a_string.find(ULIGHT) != std::string::npos)
				{
					return Light;
				}
				LOG_CRITICAL("Wrong naming convention in your shader for uniform");
				return UNDEFINED;
			}
		}
	}
}
