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
			class ENGINE_API VulkanGraphicPipeline;
		}
		namespace RHI
		{
			class ENGINE_API ILogicalDevice;
			class ENGINE_API IRenderPass;

			class ENGINE_API IGraphicPipeline
			{
			public:
				virtual ~IGraphicPipeline() = default;
				virtual GraphicsAPI::VulkanGraphicPipeline* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				virtual void Create(ILogicalDevice* a_logicalDevice, IRenderPass* a_renderPass) = 0;
				virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif 
