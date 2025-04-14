#ifndef VULKANSHADER_H
#define VULKANSHADER_H

#include <string>
#include <vulkan/vulkan_core.h>

#include "EngineExport.h"

#include "Core/Interfaces/IShader.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanShader : public RHI::IShader
			{
			public:
				ENGINE_API ~VulkanShader() override = default;
				ENGINE_API GraphicsAPI::VulkanShader* CastVulkan() override { return this; }

				ENGINE_API void Create(std::string& a_path, RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VkShaderModule GetHandle() { return m_shaderModule; }

			private:
				VkShaderModule m_shaderModule = VK_NULL_HANDLE;
			};
		}
	}
}

#endif