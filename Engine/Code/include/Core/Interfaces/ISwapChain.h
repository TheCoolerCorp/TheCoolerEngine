#ifndef ISWAPCHAIN_H
#define ISWAPCHAIN_H

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanSwapchain;
		}

		namespace RHI
		{
			class ISurface;

			class ENGINE_API ISwapChain
			{
			public:
				virtual ~ISwapChain() = default;

				GraphicsAPI::VulkanSwapchain* CastVulkan() {return nullptr;}

				virtual void Create(ISurface* a_surface) = 0;
			};
		}
	}
}

#endif