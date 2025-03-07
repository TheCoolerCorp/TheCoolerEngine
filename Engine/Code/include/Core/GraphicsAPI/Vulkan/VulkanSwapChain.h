#ifndef IVULKANSWAPCHAIN_H
#define IVULKANSWAPCHAIN_H

#include "Core/Interfaces/ISwapChain.h"

#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include <vector>


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

				void Create(RHI::ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logical_device) override;
				void Destroy(RHI::ILogicalDevice* a_logical_device) override;

				uint32_t mMaxFrame = 0;

			private:
				VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

				std::vector<VkImage> m_images = std::vector<VkImage>(0);
				std::vector<VkImageView> m_imageViews = std::vector<VkImageView>(0);

				std::vector<VkFramebuffer> m_framebuffers = std::vector<VkFramebuffer>(0);

				VkFormat m_swapChainImageFormat;
				VkExtent2D m_swapChainExtent;

				std::vector<VkSemaphore> m_imageAvailableSemaphores;
				std::vector<VkSemaphore> m_renderFinishedSemaphores;
				ENGINE_API std::vector<VkFence> m_inFlightFences;

				uint32_t m_currentFrame = 0;

				uint32_t m_imageIndex;


				void CreateSwapChain(VkSurfaceKHR a_surface);
				VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats);
				VkPresentModeKHR ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& a_availablePresentModes);
				VkExtent2D ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window);

				VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device);

			};
		}
	}
}

#endif