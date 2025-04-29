#ifndef VULKANSHADER_H
#define VULKANSHADER_H

#include <string>
#include <vulkan/vulkan_core.h>

#include "EngineExport.h"

#include "Core/Interfaces/IShader.h"

#include "spirv_reflect.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
				struct VulkanBinding
				{
					uint32_t mIndex = -1;
					std::string mName = "";
					VkDescriptorType mType;
					VkShaderStageFlags mFlags;
					uint32_t mCount = -1;

				};

				struct VulkanSet
				{
					uint32_t mIndex = -1;
					std::vector<VulkanBinding> mBindings = std::vector<VulkanBinding>(0);
				};


			class VulkanShader : public RHI::IShader
			{
			public:
				ENGINE_API ~VulkanShader() override = default;
				ENGINE_API GraphicsAPI::VulkanShader* CastVulkan() override { return this; }

				ENGINE_API void Create(std::string a_path, RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VkShaderModule GetHandle() { return m_shaderModule; }
				ENGINE_API VkShaderStageFlagBits GetFlags() { return m_FlagsBits; }
				ENGINE_API std::vector<VulkanSet> GetSets() { return m_sets; }

			private:
				VkShaderModule m_shaderModule = VK_NULL_HANDLE;
				VkShaderStageFlagBits m_FlagsBits;
				std::vector<VulkanSet> m_sets = std::vector<VulkanSet>(0);
			};
		}
	}
}

#endif