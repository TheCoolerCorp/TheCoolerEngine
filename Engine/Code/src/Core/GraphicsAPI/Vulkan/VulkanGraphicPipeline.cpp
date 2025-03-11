#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/Interfaces/ILogicalDevice.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
				void VulkanGraphicPipeline::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IRenderPass* a_renderPass)
				{
					// Shader creation
					VkShaderModule t_vertexShader = CreateShader("Shaders/frag.spv", a_logicalDevice->CastVulkan()->GetVkDevice());
					VkShaderModule t_fragmentShader = CreateShader("Shaders/vert.spv", a_logicalDevice->CastVulkan()->GetVkDevice());

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
					// VERTEX CURRENTLY NOT IMPLEMENTED
					/*auto bindingDescription = Vertex::getBindingDescription();
					auto attributeDescriptions = Vertex::getAttributeDescriptions();
					vertexInputInfo.vertexBindingDescriptionCount = 1;
					vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
					vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
					vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();*/


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
					rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
					rasterizer.depthBiasEnable = VK_FALSE;

					// Depth activate by default, need to be modular later
					VkPipelineDepthStencilStateCreateInfo depthStencil{};
					depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
					depthStencil.depthTestEnable = VK_TRUE;
					depthStencil.depthWriteEnable = VK_TRUE;
					depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
					depthStencil.depthBoundsTestEnable = VK_FALSE;
					depthStencil.stencilTestEnable = VK_FALSE;


					// Pipeline creation
					VkGraphicsPipelineCreateInfo pipelineInfo{};
					pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
					pipelineInfo.stageCount = 2;
					pipelineInfo.pStages = shaderStages;
					pipelineInfo.pVertexInputState = &vertexInputInfo;
					pipelineInfo.pInputAssemblyState = &inputAssembly;
					pipelineInfo.pViewportState = &viewportState;
					pipelineInfo.pRasterizationState = &rasterizer;
					//pipelineInfo.pMultisampleState = &multisampling;
					//pipelineInfo.pDepthStencilState = nullptr; // Optional
					//pipelineInfo.pColorBlendState = &colorBlending;
					//pipelineInfo.pDynamicState = &dynamicState;
					//pipelineInfo.layout = pipelineLayout;
					//pipelineInfo.renderPass = vkRenderPass->renderPass;
					//pipelineInfo.subpass = 0;
					//pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
					//pipelineInfo.basePipelineIndex = -1; // Optional
					//pipelineInfo.pDepthStencilState = &depthStencil;

					VK_CHECK(vkCreateGraphicsPipelines(a_logicalDevice->CastVulkan()->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline), "Failed to create graphic pipeline");


					// Destroy shader, already load don't need to store them
					vkDestroyShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), t_vertexShader, nullptr);
					vkDestroyShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), t_fragmentShader, nullptr);
				}

				void VulkanGraphicPipeline::Destroy(RHI::ILogicalDevice* a_logicalDevice)
				{
					vkDestroyPipeline(a_logicalDevice->CastVulkan()->GetVkDevice(), m_pipeline, nullptr);
					vkDestroyPipelineLayout(a_logicalDevice->CastVulkan()->GetVkDevice(), m_layout, nullptr);
				}

				VkShaderModule VulkanGraphicPipeline::CreateShader(const std::string& a_path, VkDevice a_device)
				{
					const std::vector<char>& code = ReadFile(a_path);

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
