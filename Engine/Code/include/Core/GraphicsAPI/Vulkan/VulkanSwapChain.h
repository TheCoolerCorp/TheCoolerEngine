#ifndef IVULKANSWAPCHAIN_H
#define IVULKANSWAPCHAIN_H

#include "Core/Interfaces/ISwapChain.h"

#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include  <iostream>
#include <vector>

#include "Core/Window/IWindow.h"

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

				uint32_t mMaxFrame = 0;

			private:
				VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

				void CreateSwapChain(VkSurfaceKHR a_surface);
				VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> a_availableFormats);
				VkSurfaceFormatKHR ChooseSurfacePresentMode(std::vector<VkSurfaceFormatKHR> a_availablePresentMode);
				VkSurfaceFormatKHR ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window);

			};
		}
	}
}

#endif