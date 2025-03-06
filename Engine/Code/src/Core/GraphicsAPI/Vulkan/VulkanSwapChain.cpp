#include  "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"

#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/Interfaces/ISurface.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanSwapchain::Create(RHI::ISurface* a_surface)
			{
				VkSwapchainCreateInfoKHR createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

				createInfo.surface = a_surface->CastVulkan()->GetVkSurfaceKHR();
				//createInfo.imageCount;
				//createInfo.imageFormat = surfaceFormat.format;
				//createInfo.imageColorSpace = surfaceFormat.colorSpace;
				//createInfo.imageExtent = extent;
				//createInfo.imageArrayLayers = 1;
				//createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				createInfo.oldSwapchain = VK_NULL_HANDLE;
			}

		}
	}
}
