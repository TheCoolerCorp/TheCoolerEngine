#include "Core/GraphicsAPI/Vulkan/VulkanShader.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

#include "Core/Utils.h"
#include "Core/Assertion/Assertion.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanShader::Create(std::string a_path, RHI::ILogicalDevice* a_logicalDevice)
			{
				const std::vector<char>& code = Utils::ReadFile(a_path);

				VkShaderModuleCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

				VK_CHECK(vkCreateShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), &createInfo, nullptr, &m_shaderModule), "Failed to create shader");


				SpvReflectShaderModule module;
				SpvReflectResult result = spvReflectCreateShaderModule(code.size() * sizeof(char), code.data(), &module);
				ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "Can't reflect shader");

				uint32_t set_count = 0;
				result = spvReflectEnumerateDescriptorSets(&module, &set_count, nullptr);
				std::vector<SpvReflectDescriptorSet*> reflect_sets(set_count);
				spvReflectEnumerateDescriptorSets(&module, &set_count, reflect_sets.data());

				m_FlagsBits = static_cast<VkShaderStageFlagBits>(module.shader_stage);
				m_sets.resize(set_count);
				for (int i = 0; i < m_sets.size(); ++i)
				{
					m_sets[i].mIndex = reflect_sets[i]->set;
					m_sets[i].mBindings.resize(reflect_sets[i]->binding_count);

					for (int j = 0; j < m_sets[i].mBindings.size(); ++j)
					{
						m_sets[i].mBindings[j].mIndex = reflect_sets[i]->bindings[j]->binding;
						m_sets[i].mBindings[j].mName = reflect_sets[i]->bindings[j]->name;
						m_sets[i].mBindings[j].mType = static_cast<VkDescriptorType>(reflect_sets[i]->bindings[j]->descriptor_type);
						m_sets[i].mBindings[j].mCount = reflect_sets[i]->bindings[j]->count;
						m_sets[i].mBindings[j].mFlags = static_cast<VkShaderStageFlags>(module.shader_stage);
					}
				}
				spvReflectDestroyShaderModule(&module);
			}

			void VulkanShader::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				vkDestroyShaderModule(a_logicalDevice->CastVulkan()->GetVkDevice(), m_shaderModule, nullptr);
			}

		}
	}
}
