#ifndef VULKANGRAPHICPIPELINE_H
#define VULKANGRAPHICPIPELINE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Utils.h"
#include "Core/Interfaces/IGraphicPipeline.h"
namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanGraphicPipeline : public RHI::IGraphicPipeline
			{
			public:
				ENGINE_API ~VulkanGraphicPipeline() override = default;
				ENGINE_API VulkanGraphicPipeline* CastVulkan() override{ return this; }

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IRenderPass* a_renderPass, std::array<RHI::IShader*, 2> a_vertFragShaders, std::vector<RHI::IShader*> a_additionalShaders = {}) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VkPipeline GetPipeline() const { return m_pipeline; }
				ENGINE_API VkPipelineLayout GetLayout() const { return m_layout; }
				ENGINE_API VkDescriptorSetLayout GetObjectDescriptorSetLayout() const { return m_objectDescriptor; }
				ENGINE_API VkDescriptorSetLayout GetCommontDescriptorSetLayout() const { return m_commonDescriptor; }

			private:
				VkShaderModule CreateShader(const std::string& a_path, VkDevice a_device);

				VkPipeline m_pipeline = VK_NULL_HANDLE;
				VkPipelineLayout m_layout = VK_NULL_HANDLE;
				VkDescriptorSetLayout m_objectDescriptor = VK_NULL_HANDLE;
				VkDescriptorSetLayout m_commonDescriptor = VK_NULL_HANDLE;
			};
		}
	}
}

#endif 
