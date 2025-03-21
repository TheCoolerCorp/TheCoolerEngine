#ifndef ICOMMANDPOOL_H
#define ICOMMANDPOOL_H

#include <vector>

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanCommandPool;
		}

		namespace RHI
		{
			class ISurface;
			class ILogicalDevice;
			class IPhysicalDevice;
			class IRenderPass;
			class ISwapChain;
			class IGraphicPipeline;

			class ICommandPool
			{
			public:
				ENGINE_API virtual ~ICommandPool() = default;
				ENGINE_API virtual void Create(IPhysicalDevice* a_physicalDevice, ISurface* a_surface, ILogicalDevice* a_logicalDevice) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
				ENGINE_API virtual void CreateCommandBuffer(ILogicalDevice* a_logicalDevice, ISwapChain* a_swapChain, IRenderPass* a_renderPass, IGraphicPipeline* a_graphicPipeline) = 0;
				ENGINE_API virtual GraphicsAPI::VulkanCommandPool* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanCommandPool!"); return nullptr; }
			};
		}
	}
}

#endif