#ifndef IRENDERPASS_H
#define IRENDERPASS_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanRenderPass;
		}
		namespace RHI
		{
			class ISwapChain;
			class ILogicalDevice;
			class IPhysicalDevice;
			class ICommandPool;

			class IRenderPass
			{
			public:
				ENGINE_API virtual ~IRenderPass() = default;
				ENGINE_API virtual GraphicsAPI::VulkanRenderPass* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(ISwapChain* a_swapChain, IPhysicalDevice* a_physicalDevice, ILogicalDevice* a_logicalDevice) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;

				ENGINE_API virtual void BeginRenderPass(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, RHI::ISwapChain* a_swapChain, uint32_t a_imageIndex) = 0;
				ENGINE_API virtual void EndRenderPass(RHI::ICommandPool* a_commandPool, uint32_t a_commandBufferIndex, uint32_t a_currentFrame) = 0;
			};
		}
	}
}
#endif