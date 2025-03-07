#include  "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"

#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/Interfaces/ISurface.h"
#include "Core/Window/GLWindow.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanSwapchain::Create(RHI::ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logical_device)
			{
				uint32_t imageCount = 0;
				GraphicsAPI::VulkanSurface::SurfaceInfo info = a_surface->CastVulkan()->GetSurfaceInfo();
				if (mMaxFrame <= 0)
				{
					imageCount = info.capabilities.minImageCount + 1;

					if (info.capabilities.maxImageCount > 0 && imageCount > info.capabilities.maxImageCount) 
					{
						imageCount = info.capabilities.maxImageCount;
					}
				}
				else
				{
					imageCount = mMaxFrame;
				}

				VkSurfaceFormatKHR t_formats = ChooseSurfaceFormat(info.formats);
				VkPresentModeKHR t_presentMode = ChooseSurfacePresentMode(info.presentModes);
				VkExtent2D t_extent = ChooseSurfaceExtent(info.capabilities, a_window);


				VkSwapchainCreateInfoKHR createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

				createInfo.surface = a_surface->CastVulkan()->GetVkSurfaceKHR();
				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = t_formats.format;
				createInfo.imageColorSpace = t_formats.colorSpace;
				createInfo.imageExtent = t_extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

				QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(a_physicalDevice->CastVulkan()->GetVkPhysicalDevice(), a_surface->CastVulkan()->GetVkSurfaceKHR());
				uint32_t queueFamilyIndices[] = { indices.GetGraphicsFamily().value(), indices.GetPresentFamily().value() };

				if (indices.GetGraphicsFamily() != indices.GetPresentFamily())
				{
					createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					createInfo.queueFamilyIndexCount = 2;
					createInfo.pQueueFamilyIndices = queueFamilyIndices;
				}
				else
				{
					createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				}

				createInfo.preTransform = info.capabilities.currentTransform;
				createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				createInfo.presentMode = t_presentMode;
				createInfo.clipped = VK_TRUE;

				createInfo.oldSwapchain = VK_NULL_HANDLE;

				VK_CHECK(vkCreateSwapchainKHR(a_logical_device->CastVulkan()->GetVkDevice(), &createInfo, nullptr, &m_swapChain), "failed to create swap chain!");

				vkGetSwapchainImagesKHR(a_logical_device->CastVulkan()->GetVkDevice(), m_swapChain, &imageCount, nullptr);
				m_images.resize(imageCount);
				vkGetSwapchainImagesKHR(a_logical_device->CastVulkan()->GetVkDevice(), m_swapChain, &imageCount, m_images.data());

				m_swapChainImageFormat = t_formats.format;
				m_swapChainExtent = t_extent;

				// Create imagesViews;
				for (int i = 0; i < m_images.size(); ++i)
				{
					m_imageViews[i] = CreateImageView(m_images[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, a_logical_device->CastVulkan()->GetVkDevice());
				}

			}

			void VulkanSwapchain::Destroy(RHI::ILogicalDevice* a_logical_device)
			{
				for (size_t i = 0; i < m_framebuffers.size(); i++) {
					vkDestroyFramebuffer(a_logical_device->CastVulkan()->GetVkDevice(), m_framebuffers[i], nullptr);
				}

				for (size_t i = 0; i < m_imageViews.size(); i++) {
					vkDestroyImageView(a_logical_device->CastVulkan()->GetVkDevice(), m_imageViews[i], nullptr);
				}

				vkDestroySwapchainKHR(a_logical_device->CastVulkan()->GetVkDevice(), m_swapChain, nullptr);
			}

			VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats)
			{
				for (const auto& availableFormat : a_availableFormats)
				{
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
					{
						return availableFormat;
					}
				}

				return a_availableFormats[0];
			}

			VkPresentModeKHR VulkanSwapchain::ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& a_availablePresentModes)
			{
				for (const auto& availablePresentMode : a_availablePresentModes)
				{
					if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
					{
						return availablePresentMode;
					}
				}
				return VK_PRESENT_MODE_FIFO_KHR;
			}


			VkExtent2D VulkanSwapchain::ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window)
			{
				if (a_availableCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
				{
					return a_availableCapabilities.currentExtent;
				}
				else 
				{
					int width, height = 0;
					a_window->CastGLFW()->GetFramebufferSize(&width, &height);

					VkExtent2D actualExtent = 
					{
						static_cast<uint32_t>(width),
						static_cast<uint32_t>(height)
					};

					actualExtent.width = std::clamp(actualExtent.width, a_availableCapabilities.minImageExtent.width, a_availableCapabilities.maxImageExtent.width);
					actualExtent.height = std::clamp(actualExtent.height, a_availableCapabilities.minImageExtent.height, a_availableCapabilities.maxImageExtent.height);

					return actualExtent;
				}
			}

			VkImageView VulkanSwapchain::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device)
			{
				VkImageViewCreateInfo viewInfo{};
				viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image = image;
				viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				viewInfo.format = format;
				viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.subresourceRange.baseMipLevel = 0;
				viewInfo.subresourceRange.levelCount = 1;
				viewInfo.subresourceRange.baseArrayLayer = 0;
				viewInfo.subresourceRange.layerCount = 1;
				viewInfo.subresourceRange.aspectMask = aspectFlags;

				VkImageView imageView;

				if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
					throw std::runtime_error("failed to create texture image view!");
				}

				return imageView;
			}

		}
	}
}
