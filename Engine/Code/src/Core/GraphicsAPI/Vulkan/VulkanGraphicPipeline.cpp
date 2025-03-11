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

					VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
					vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;




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
