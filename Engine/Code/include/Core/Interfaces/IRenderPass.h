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
			class ENGINE_API VulkanRenderPass;
		}
		namespace RHI
		{
			class ISwapChain;
			class ILogicalDevice;
			class IPhysicalDevice;

			class ENGINE_API IRenderPass
			{
			public:
				virtual ~IRenderPass() = default;
				virtual GraphicsAPI::VulkanRenderPass* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				virtual void Create(ISwapChain* a_swapChain, IPhysicalDevice* a_physicalDevice, ILogicalDevice* a_logicalDevice) = 0;
				virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}
#endif