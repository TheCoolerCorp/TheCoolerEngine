#ifndef IVULKANSWAPCHAIN_H
#define IVULKANSWAPCHAIN_H

#include "Core/Interfaces/ISwapChain.h"

#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanSwapchain : public RHI::ISwapChain
			{
			public:
				~VulkanSwapchain() override = default;

				void Create(RHI::ISurface* a_surface) override;

			private:
				VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

				void CreateSwapChain(VkSurfaceKHR a_surface);

			};
		}
	}
}

#endif