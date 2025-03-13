#include  "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"

#include "Core/Assertion/Assertion.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/Interfaces/ISurface.h"
#include "Core/Window/GLWindow.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanImage.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			struct VulkanSwapchain::Vectors
			{
				std::vector<VkImage> mImages = std::vector<VkImage>(0);
				std::vector<VkImageView> mImageViews = std::vector<VkImageView>(0);

				std::vector<VkFramebuffer> mFramebuffers = std::vector<VkFramebuffer>(0);

				std::vector<VkSemaphore> mImageAvailableSemaphores;
				std::vector<VkSemaphore> mRenderFinishedSemaphores;
				std::vector<VkFence> mInFlightFences;
			};

			VulkanSwapchain::VulkanSwapchain() : m_swapChainImageFormat(), m_swapChainExtent(),
			                                     m_vectorsStruct(new Vectors), m_imageIndex(0) {}

			VulkanSwapchain::~VulkanSwapchain()
			{
				delete m_vectorsStruct;
			}

			void VulkanSwapchain::Create(RHI::ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice)
			{
				uint32_t t_imageCount = 0;
				const VulkanSurface::SurfaceInfo t_info = a_surface->CastVulkan()->GetSurfaceInfo();
				if (mMaxFrame <= 0)
				{
					t_imageCount = t_info.mCapabilities.minImageCount + 1;

					if (t_info.mCapabilities.maxImageCount > 0 && t_imageCount > t_info.mCapabilities.maxImageCount) 
					{
						t_imageCount = t_info.mCapabilities.maxImageCount;
					}
				}
				else
				{
					t_imageCount = mMaxFrame;
				}

				const VkSurfaceFormatKHR t_formats = ChooseSurfaceFormat(t_info.mFormats);
				const VkPresentModeKHR t_presentMode = ChooseSurfacePresentMode(t_info.mPresentModes);
				const VkExtent2D t_extent = ChooseSurfaceExtent(t_info.mCapabilities, a_window);


				VkSwapchainCreateInfoKHR t_createInfo = {};
				t_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

				t_createInfo.surface = a_surface->CastVulkan()->GetVkSurfaceKHR();
				t_createInfo.minImageCount = t_imageCount;
				t_createInfo.imageFormat = t_formats.format;
				t_createInfo.imageColorSpace = t_formats.colorSpace;
				t_createInfo.imageExtent = t_extent;
				t_createInfo.imageArrayLayers = 1;
				t_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

				const QueueFamilyIndices t_indices = QueueFamilyIndices::FindQueueFamilies(
					a_physicalDevice->CastVulkan()->GetVkPhysicalDevice(), a_surface->CastVulkan()->GetVkSurfaceKHR());
				const uint32_t t_queueFamilyIndices[] = {
					t_indices.GetGraphicsFamily().value(), t_indices.GetPresentFamily().value()
				};

				if (t_indices.GetGraphicsFamily() != t_indices.GetPresentFamily())
				{
					t_createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					t_createInfo.queueFamilyIndexCount = 2;
					t_createInfo.pQueueFamilyIndices = t_queueFamilyIndices;
				}
				else
				{
					t_createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
					t_createInfo.queueFamilyIndexCount = 0;
					t_createInfo.pQueueFamilyIndices = nullptr;
				}

				t_createInfo.preTransform = t_info.mCapabilities.currentTransform;
				t_createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				t_createInfo.presentMode = t_presentMode;
				t_createInfo.clipped = VK_TRUE;

				t_createInfo.oldSwapchain = VK_NULL_HANDLE;

				VK_CHECK(vkCreateSwapchainKHR(a_logicalDevice->CastVulkan()->GetVkDevice(), &t_createInfo, nullptr, &m_swapChain), "failed to create swap chain!");

				vkGetSwapchainImagesKHR(a_logicalDevice->CastVulkan()->GetVkDevice(), m_swapChain, &t_imageCount, nullptr);
				m_vectorsStruct->mImages.resize(t_imageCount);
				m_vectorsStruct->mImageViews.resize(t_imageCount);
				vkGetSwapchainImagesKHR(a_logicalDevice->CastVulkan()->GetVkDevice(), m_swapChain, &t_imageCount, m_vectorsStruct->mImages.data());

				m_swapChainImageFormat = t_formats.format;
				m_swapChainExtent = t_extent;

				// Create imagesViews;
				for (int i = 0; i < m_vectorsStruct->mImages.size(); ++i)
				{
					VulkanImage::CreateImageView(&m_vectorsStruct->mImages[i], &m_vectorsStruct->mImageViews[i], a_logicalDevice->CastVulkan()->GetVkDevice(), m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);


					m_depthFormat = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
																									VK_IMAGE_TILING_OPTIMAL,
																									VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
					VulkanImage::CreateImage(&m_depthImage, &m_depthMemory, a_logicalDevice->CastVulkan()->GetVkDevice(), a_physicalDevice->CastVulkan()->GetVkPhysicalDevice(), m_swapChainExtent.width, m_swapChainExtent.height, m_depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
					VulkanImage::CreateImageView(&m_depthImage, &m_depthImageView, a_logicalDevice->CastVulkan()->GetVkDevice(), m_depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
				}

			}

			void VulkanSwapchain::CreateFramebuffers(RHI::ILogicalDevice* a_logicalDevice, RHI::IRenderPass* a_renderPass)
			{
				m_vectorsStruct->mFramebuffers.resize(m_vectorsStruct->mImageViews.size());

				for (size_t i = 0; i < m_vectorsStruct->mFramebuffers.size(); i++)
				{
					std::vector<VkImageView> attachments;

					attachments.push_back(m_vectorsStruct->mImageViews[i]);
					attachments.push_back(m_depthImageView);

					VkFramebufferCreateInfo framebufferInfo{};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = a_renderPass->CastVulkan()->GetRenderPass();
					framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = m_swapChainExtent.width;
					framebufferInfo.height = m_swapChainExtent.height;
					framebufferInfo.layers = 1;

					VK_CHECK(vkCreateFramebuffer(a_logicalDevice->CastVulkan()->GetVkDevice(), &framebufferInfo, nullptr, &m_vectorsStruct->mFramebuffers[i]), "Failed to create framebuffers in swapchain");
				}
			}

			void VulkanSwapchain::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				for (size_t i = 0; i < m_vectorsStruct->mFramebuffers.size(); i++) {
					vkDestroyFramebuffer(a_logicalDevice->CastVulkan()->GetVkDevice(), m_vectorsStruct->mFramebuffers[i], nullptr);
				}

				for (size_t i = 0; i < m_vectorsStruct->mImageViews.size(); i++) {
					vkDestroyImageView(a_logicalDevice->CastVulkan()->GetVkDevice(), m_vectorsStruct->mImageViews[i], nullptr);
				}

				vkDestroySwapchainKHR(a_logicalDevice->CastVulkan()->GetVkDevice(), m_swapChain, nullptr);
			}

			VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats)
			{
				ASSERT(!a_availableFormats.empty(), "No available formats !");
				for (const auto& t_availableFormat : a_availableFormats)
				{
					if (t_availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && t_availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
					{
						return t_availableFormat;
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
				int t_width, t_height = 0;
				a_window->CastGLFW()->GetFramebufferSize(&t_width, &t_height);

				VkExtent2D actualExtent = 
				{
					static_cast<uint32_t>(t_width),
					static_cast<uint32_t>(t_height)
				};

				actualExtent.width = std::clamp(actualExtent.width, a_availableCapabilities.minImageExtent.width, a_availableCapabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, a_availableCapabilities.minImageExtent.height, a_availableCapabilities.maxImageExtent.height);

				return actualExtent;
			}

			VkImageView VulkanSwapchain::CreateImageView(const VkImage a_image, const VkFormat a_format, const VkImageAspectFlags a_aspectFlags, const VkDevice a_device)
			{
				VkImageViewCreateInfo t_viewInfo{};
				t_viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				t_viewInfo.image = a_image;
				t_viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				t_viewInfo.format = a_format;
				t_viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				t_viewInfo.subresourceRange.baseMipLevel = 0;
				t_viewInfo.subresourceRange.levelCount = 1;
				t_viewInfo.subresourceRange.baseArrayLayer = 0;
				t_viewInfo.subresourceRange.layerCount = 1;
				t_viewInfo.subresourceRange.aspectMask = a_aspectFlags;

				VkImageView t_imageView;

				VK_CHECK(vkCreateImageView(a_device, &t_viewInfo, nullptr, &t_imageView), "failed to create texture image view!")

				return t_imageView;
			}
		}
	}
}
