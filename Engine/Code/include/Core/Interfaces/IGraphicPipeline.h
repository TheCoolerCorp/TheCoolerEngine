#ifndef IGRAPHICPIPELINE_H
#define IGRAPHICPIPELINE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanGraphicPipeline;
		}
		namespace RHI
		{
			class ILogicalDevice;
			class IRenderPass;
			class IShader;

			enum PipelineType : int
			{
				Unlit,
				Lit,
				Other
			};

			class IGraphicPipeline
			{
			public:
				ENGINE_API virtual ~IGraphicPipeline() = default;
				ENGINE_API virtual GraphicsAPI::VulkanGraphicPipeline* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(ILogicalDevice* a_logicalDevice, IRenderPass* a_renderPass, PipelineType a_type, std::array<IShader*, 2> a_vertFragShaders, std::vector<IShader*> a_additionalShaders = {}) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif 
