#ifndef IVULKANSWAPCHAIN_H
#define IVULKANSWAPCHAIN_H

#include "Core/Interfaces/ISwapChain.h"
#include  "Core/Interfaces/IRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/GraphicsAPI/Vulkan/VulkanImage.h"
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

				void CreateFramebuffers(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::ICommandPool* a_commandPool) override;

				GraphicsAPI::VulkanSwapchain* CastVulkan() override { return this; }

				VkFormat GetImageFormat() const { return m_swapChainImageFormat; }
				std::vector<VkFramebuffer> GetFramebuffers() const;
				VkExtent2D GetExtent2D() const { return m_swapChainExtent; }

				uint32_t mMaxFrame = 0;

				int GetMaxFrame() override { return static_cast<uint32_t>(mMaxFrame); }

			private:
				VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

				VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D m_swapChainExtent;

				struct Vectors;
				Vectors* m_vectorsStruct;

				uint32_t m_currentFrame = 0;

				uint32_t m_imageIndex;

				// Depth
				VkImage m_depthImage = VK_NULL_HANDLE;
				VkImageView m_depthImageView = VK_NULL_HANDLE;
				VkDeviceMemory m_depthMemory = VK_NULL_HANDLE;
				VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;

				static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats);
				static VkPresentModeKHR ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& a_availablePresentModes);
				static VkExtent2D ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window);

				static VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags, VkDevice a_device);

			};
		}
	}
}

#endif