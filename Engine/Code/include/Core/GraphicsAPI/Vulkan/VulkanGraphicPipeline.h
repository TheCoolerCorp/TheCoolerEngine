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
			#define COMMON std::string("common")
			#define PER std::string("per")
			#define ULIGHT std::string("light")

			enum SetLayoutType : int
			{
				Common = 0,
				Per = 1,
				Light = 2,
				UNDEFINED = 3
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

				ENGINE_API void Bind(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, RHI::ISwapChain* a_swapChain) override;

				ENGINE_API void BindObjects(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, uint32_t a_currentFrame, uint32_t  a_imageIndex, std::vector<RHI::IBuffer*> a_indexBuffers,
											std::vector<RHI::IBuffer*> a_vertexBuffers, std::vector<uint32_t> a_indicesCount, std::vector<RHI::IObjectDescriptor*> a_objectsDescriptors) override;

				ENGINE_API void BindSingleDescriptors(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, uint32_t a_currentFrame, uint32_t  a_imageIndex, std::vector<RHI::IObjectDescriptor*> a_objectsDescriptors) override;

				ENGINE_API VkPipeline GetPipeline() const { return m_pipeline; }
				ENGINE_API VkPipelineLayout GetLayout() const { return m_layout; }

				ENGINE_API std::vector<VulkanSetLayout> GetSetLayouts() const { return m_setslayouts; }

			private:
				SetLayoutType GetType(std::string a_string);

				VkPipeline m_pipeline = VK_NULL_HANDLE;
				VkPipelineLayout m_layout = VK_NULL_HANDLE;

				std::vector<VulkanSetLayout> m_setslayouts = std::vector<VulkanSetLayout>();
			};
		}
	}
}

#endif 
