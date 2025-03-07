#ifndef ISWAPCHAIN_H
#define ISWAPCHAIN_H

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			class ENGINE_API IWindow;
		}
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanSwapchain;
		}

		namespace RHI
		{
			class ISurface;
			class IPhysicalDevice;
			class ILogicalDevice;

			class ENGINE_API ISwapChain
			{
			public:
				virtual ~ISwapChain() = default;

				GraphicsAPI::VulkanSwapchain* CastVulkan() {return nullptr;}

				virtual void Create(ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logical_device) = 0;
				virtual void Destroy(RHI::ILogicalDevice* a_logical_device) = 0;
			};
		}
	}
}

#endif