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
				ENGINE_API virtual void CreateCommandBuffer(RHI::ILogicalDevice* a_logicalDevice, uint32_t a_maxFrames) = 0;

				ENGINE_API virtual void BeginCommand(uint32_t a_commandBufferIndex, uint32_t a_currentFrame) = 0;
				ENGINE_API virtual void EndCommand(uint32_t a_commandBufferIndex, uint32_t a_currentFrame) = 0;

				ENGINE_API virtual GraphicsAPI::VulkanCommandPool* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanCommandPool!"); return nullptr; }

			protected:
				static std::mutex m_commandsMutex;
			};
		}
	}
}

#endif