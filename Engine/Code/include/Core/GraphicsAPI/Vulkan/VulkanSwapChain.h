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
			class VulkanSwapchain : public RHI::ISwapChain
			{
			public:
				ENGINE_API VulkanSwapchain();
				ENGINE_API ~VulkanSwapchain() override;

				ENGINE_API void Create(RHI::ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API void CreateFramebuffers(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::ICommandPool* a_commandPool) override;

				ENGINE_API VulkanSwapchain* CastVulkan() override { return this; }

				ENGINE_API VkFormat GetImageFormat() const { return m_swapChainImageFormat; }
				ENGINE_API std::vector<VkFramebuffer> GetFramebuffers() const;
				ENGINE_API VkExtent2D GetExtent2D() const { return m_swapChainExtent; }


				ENGINE_API int GetMaxFrame() override { return static_cast<uint32_t>(m_maxFrame); }

			private:
				VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

				VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D m_swapChainExtent;

				struct Vectors;
				Vectors* m_vectorsStruct;

				uint32_t m_currentFrame = 0;

				uint32_t m_imageIndex;

				uint32_t m_maxFrame = 0;

				// Depth
				VkImage m_depthImage = VK_NULL_HANDLE;
				VkImageView m_depthImageView = VK_NULL_HANDLE;
				VkDeviceMemory m_depthMemory = VK_NULL_HANDLE;
				VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;

				ENGINE_API static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats);
				ENGINE_API static VkPresentModeKHR ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& a_availablePresentModes);
				ENGINE_API static VkExtent2D ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window);

				ENGINE_API static VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags, VkDevice a_device);

			};
		}
	}
}

#endif