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
			#define COMMON std::string("per")
			#define PER std::string("common")

			enum SetLayoutType : int
			{
				Common = 0,
				Per = 1,
				UNDEFINED = 2
			};

			struct VulkanSetLayout
			{
				uint32_t mIndex = -1;

				SetLayoutType mType = SetLayoutType::Common;

				VkDescriptorSetLayout mLayout;
			};

			class VulkanGraphicPipeline : public RHI::IGraphicPipeline
			{
			public:
				ENGINE_API ~VulkanGraphicPipeline() override = default;
				ENGINE_API VulkanGraphicPipeline* CastVulkan() override{ return this; }

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IRenderPass* a_renderPass, RHI::PipelineType a_type, std::array<RHI::IShader*, 2> a_vertFragShaders, std::vector<RHI::IShader*> a_additionalShaders = {}) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VkPipeline GetPipeline() const { return m_pipeline; }
				ENGINE_API VkPipelineLayout GetLayout() const { return m_layout; }
				ENGINE_API VkDescriptorSetLayout GetObjectDescriptorSetLayout() const { return m_objectDescriptor; }
				ENGINE_API VkDescriptorSetLayout GetCommontDescriptorSetLayout() const { return m_commonDescriptor; }

				ENGINE_API std::vector<VulkanSetLayout> GetSetLayouts() const { return m_setslayouts; }

				ENGINE_API RHI::PipelineType GetType() { return m_type; }
			private:
				SetLayoutType GetType(std::string a_string);

				VkPipeline m_pipeline = VK_NULL_HANDLE;
				VkPipelineLayout m_layout = VK_NULL_HANDLE;
				VkDescriptorSetLayout m_objectDescriptor = VK_NULL_HANDLE;
				VkDescriptorSetLayout m_commonDescriptor = VK_NULL_HANDLE;

				std::vector<VulkanSetLayout> m_setslayouts = std::vector<VulkanSetLayout>();

				RHI::PipelineType m_type = RHI::Unlit;
			};
		}
	}
}

#endif 
