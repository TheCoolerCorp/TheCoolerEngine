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
				uint32_t imageCount = 0;
				GraphicsAPI::VulkanSurface::SurfaceInfo info = a_surface->CastVulkan()->GetSurfaceInfo();
				if (mMaxFrame <= 0)
				{
					imageCount = info.capabilities.minImageCount + 1;

					if (info.capabilities.maxImageCount > 0 && imageCount > info.capabilities.maxImageCount) {
						imageCount = info.capabilities.maxImageCount;
					}
				}
				else
				{
					imageCount = mMaxFrame;
				}



				VkSwapchainCreateInfoKHR createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

				createInfo.surface = a_surface->CastVulkan()->GetVkSurfaceKHR();
				createInfo.minImageCount = imageCount;
				//createInfo.imageFormat = info.format;
				//createInfo.imageColorSpace = surfaceFormat.colorSpace;
				//createInfo.imageExtent = extent;
				//createInfo.imageArrayLayers = 1;
				//createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				createInfo.oldSwapchain = VK_NULL_HANDLE;
			}

		}
	}
}
