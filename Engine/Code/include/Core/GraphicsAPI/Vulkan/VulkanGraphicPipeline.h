#ifndef VULKANGRAPHICPIPELINE_H
#define VULKANGRAPHICPIPELINE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Utils.h"
#include "Core/Interfaces/IGraphicPipeline.h"
#include "Ressources/Vertex.h"
#include <array>
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

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IRenderPass* a_renderPass) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VkPipeline GetPipeline() const { return m_pipeline; }

			private:
				VkShaderModule CreateShader(const std::string& a_path, VkDevice a_device);

				VkPipeline m_pipeline = VK_NULL_HANDLE;
				VkPipelineLayout m_layout = VK_NULL_HANDLE;
				VkDescriptorSetLayout m_descriptor = VK_NULL_HANDLE;
			};
		}
	}
}

#endif 
