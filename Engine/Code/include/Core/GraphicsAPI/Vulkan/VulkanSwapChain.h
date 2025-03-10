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
				VulkanSwapchain();
				~VulkanSwapchain() override;

				void Create(RHI::ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice) override;
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				GraphicsAPI::VulkanSwapchain* CastVulkan() override { return this; }

				VkFormat GetImageFormat() { return m_swapChainImageFormat; }

				uint32_t mMaxFrame = 0;

			private:
				VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

				VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D m_swapChainExtent;

				struct Vectors;
				Vectors* m_vectorsStruct;

				uint32_t m_currentFrame = 0;

				uint32_t m_imageIndex;

				void CreateSwapChain(VkSurfaceKHR a_surface);
				static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats);
				static VkPresentModeKHR ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& a_availablePresentModes);
				static VkExtent2D ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window);

				static VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags, VkDevice a_device);

			};
		}
	}
}

#endif