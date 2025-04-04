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
				ENGINE_API VkPipelineLayout GetLayout() const { return m_layout; }
				ENGINE_API VkDescriptorSetLayout GetObjectDescriptorSetLayout() const { return m_objectDescriptor; }
				ENGINE_API VkDescriptorSetLayout GetCameraDescriptorSetLayout() const { return m_cameraDescriptor; }

				ENGINE_API static VkShaderModule CreateShader(const std::string& a_path, VkDevice a_device);

			private:
				VkPipeline m_pipeline = VK_NULL_HANDLE;
				VkPipelineLayout m_layout = VK_NULL_HANDLE;
				VkDescriptorSetLayout m_objectDescriptor = VK_NULL_HANDLE;
				VkDescriptorSetLayout m_cameraDescriptor = VK_NULL_HANDLE;
			};
		}
	}
}

#endif 
