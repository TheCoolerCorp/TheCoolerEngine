#include "Core/GraphicsAPI/Vulkan/VulkanShader.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

#include "Core/Utils.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			ENGINE_API void VulkanShader::Create(std::string& a_path, RHI::ILogicalDevice* a_logicalDevice)
			{
				const std::vector<char>& code = Utils::ReadFile(a_path);

				VkShaderModuleCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

				VK_CHECK(vkCreateShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), &createInfo, nullptr, &m_shaderModule), "Failed to create shader");
			}

			ENGINE_API void VulkanShader::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				vkDestroyShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), m_shaderModule, nullptr);
			}

		}
	}
}