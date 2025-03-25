#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/Interfaces/ILogicalDevice.h"
#include "Core/Interfaces/IRenderPass.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
				void VulkanGraphicPipeline::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IRenderPass* a_renderPass)
				{
					// Shader creation
					VkShaderModule t_vertexShader = CreateShader("Assets/Shaders/vert.spv", a_logicalDevice->CastVulkan()->GetVkDevice());
					VkShaderModule t_fragmentShader = CreateShader("Assets/Shaders/frag.spv", a_logicalDevice->CastVulkan()->GetVkDevice());

					VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
					vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
					vertShaderStageInfo.module = t_vertexShader;
					vertShaderStageInfo.pName = "main";

					VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
					fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
					fragShaderStageInfo.module = t_fragmentShader;
					fragShaderStageInfo.pName = "main";

					VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

					// Vertex setup
					VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
					vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
					auto bindingDescription = Resource::VulkanVertexSpec::getBindingDescription();
					auto attributeDescriptions = Resource::VulkanVertexSpec::getAttributeDescriptions();
					vertexInputInfo.vertexBindingDescriptionCount = 1;
					vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
					vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
					vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();


					// Pipeline spec
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
					rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
					rasterizer.depthBiasEnable = VK_FALSE;

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

					VkPipelineDynamicStateCreateInfo dynamicState{};
					dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
					dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
					dynamicState.pDynamicStates = dynamicStates.data();

					VkDescriptorSetLayoutBinding camUBOLayoutBinding{};
					camUBOLayoutBinding.binding = 0;
					camUBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					camUBOLayoutBinding.descriptorCount = 1;
					camUBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
					camUBOLayoutBinding.pImmutableSamplers = nullptr;

					VkDescriptorSetLayoutCreateInfo camUBOLayoutInfo{};
					camUBOLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
					camUBOLayoutInfo.bindingCount = 1;
					camUBOLayoutInfo.pBindings = &camUBOLayoutBinding;

					VK_CHECK(vkCreateDescriptorSetLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), &camUBOLayoutInfo, nullptr, &m_cameraDescriptor),
						"Failed to create Camera UBO descriptor set layout");

					VkDescriptorSetLayoutBinding objUBOLayoutBinding{};
					objUBOLayoutBinding.binding = 0;
					objUBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					objUBOLayoutBinding.descriptorCount = 1;
					objUBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
					objUBOLayoutBinding.pImmutableSamplers = nullptr;

					VkDescriptorSetLayoutBinding samplerLayoutBinding{};
					samplerLayoutBinding.binding = 1;
					samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					samplerLayoutBinding.descriptorCount = 1;
					samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
					samplerLayoutBinding.pImmutableSamplers = nullptr;

					std::array<VkDescriptorSetLayoutBinding, 2> objBindings = { objUBOLayoutBinding, samplerLayoutBinding };

					VkDescriptorSetLayoutCreateInfo objUBOLayoutInfo{};
					objUBOLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
					objUBOLayoutInfo.bindingCount = static_cast<uint32_t>(objBindings.size());
					objUBOLayoutInfo.pBindings = objBindings.data();

					VK_CHECK(vkCreateDescriptorSetLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), &objUBOLayoutInfo, nullptr, &m_objectDescriptor),
						"Failed to create Object UBO + Sampler descriptor set layout");

					std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = { m_cameraDescriptor, m_objectDescriptor };

					VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
					pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
					pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
					pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

					VK_CHECK(vkCreatePipelineLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), &pipelineLayoutInfo, nullptr, &m_layout),
						"Failed to create pipeline layout");

					VkGraphicsPipelineCreateInfo pipelineInfo{};
					pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
					pipelineInfo.stageCount = 2;
					pipelineInfo.pStages = shaderStages;
					pipelineInfo.pVertexInputState = &vertexInputInfo;
					pipelineInfo.pInputAssemblyState = &inputAssembly;
					pipelineInfo.pViewportState = &viewportState;
					pipelineInfo.pRasterizationState = &rasterizer;
					pipelineInfo.pMultisampleState = &multisampling;
					pipelineInfo.pColorBlendState = &colorBlending;
					pipelineInfo.pDynamicState = &dynamicState;
					pipelineInfo.layout = m_layout;
					pipelineInfo.renderPass = a_renderPass->CastVulkan()->GetRenderPass();
					pipelineInfo.subpass = 0;
					pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
					pipelineInfo.basePipelineIndex = -1;
					pipelineInfo.pDepthStencilState = &depthStencil;

					VK_CHECK(vkCreateGraphicsPipelines(a_logicalDevice->CastVulkan()->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline),
						"Failed to create graphic pipeline");


					// Destroy shader, already load don't need to store them
					vkDestroyShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), t_vertexShader, nullptr);
					vkDestroyShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), t_fragmentShader, nullptr);
				}

				void VulkanGraphicPipeline::Destroy(RHI::ILogicalDevice* a_logicalDevice)
				{
					vkDestroyDescriptorSetLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), m_objectDescriptor, nullptr);
					vkDestroyDescriptorSetLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), m_cameraDescriptor, nullptr);
					vkDestroyPipeline(a_logicalDevice->CastVulkan()->GetVkDevice(), m_pipeline, nullptr);
					vkDestroyPipelineLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), m_layout, nullptr);
				}

				VkShaderModule VulkanGraphicPipeline::CreateShader(const std::string& a_path, VkDevice a_device)
				{
					const std::vector<char>& code = Utils::ReadFile(a_path);

					VkShaderModuleCreateInfo createInfo{};
					createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
					createInfo.codeSize = code.size();
					createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

					VkShaderModule shaderModule;
					VK_CHECK(vkCreateShaderModule(a_device, &createInfo, nullptr, &shaderModule), "Failed to create shader");

					return shaderModule;
				}
		}
	}
}
