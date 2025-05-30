#ifndef IVULKANSWAPCHAIN_H
#define IVULKANSWAPCHAIN_H

#include <functional>

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
			class VulkanCommandPool;
			class VulkanPhysicalDevice;
			class VulkanSurface;
			class VulkanLogicalDevice;

			class VulkanSwapchain : public RHI::ISwapChain
			{
			public:
				ENGINE_API VulkanSwapchain();

				ENGINE_API void Create(RHI::ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void CleanupSwapChain(const VkDevice a_device) const;

				ENGINE_API void CreateFramebuffers(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::ICommandPool* a_commandPool) override;

				ENGINE_API void CreateSyncObjects(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VulkanSwapchain* CastVulkan() override { return this; }

				[[nodiscard]] ENGINE_API VkFormat GetImageFormat() const { return m_swapChainImageFormat; }
				[[nodiscard]] ENGINE_API std::vector<VkFramebuffer> GetFramebuffers() const;
				[[nodiscard]] ENGINE_API VkExtent2D GetExtent2D() const { return m_swapChainExtent; }
				[[nodiscard]] ENGINE_API const std::vector<VkImageView>& GetImageViews() { return m_imageViews; }

				ENGINE_API int GetMaxFrame() override { return static_cast<int>(m_maxFrame); }

				ENGINE_API uint32_t GetCurrentFrame() const override { return m_currentFrame; }

				ENGINE_API void BeginFrame(RHI::ILogicalDevice* a_logicalDevice, uint32_t* outImageIndex) override;
				ENGINE_API void EndFrame(RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool, RHI::ISurface* a_surface, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, Window::IWindow* a_window, uint32_t a_FrameBufferIndex) override;

				ENGINE_API void AddResizeCallback(const std::function<void(VkExtent2D a_extent)>& a_callback) { m_resizeCallback.push_back(a_callback); }

				ENGINE_API void CallResizeCallbacks(const VkExtent2D a_extent) const
				{
					for (const auto& t_callback : m_resizeCallback)
					{
						t_callback(a_extent);
					}
				}
			private:
				VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

				VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D m_swapChainExtent;

				std::vector<VkImage> m_images = std::vector<VkImage>(0);
				std::vector<VkImageView> m_imageViews = std::vector<VkImageView>(0);

				std::vector<VkFramebuffer> m_framebuffers = std::vector<VkFramebuffer>(0);

				uint32_t m_currentFrame = 0;

				uint32_t m_imageIndex;

				uint32_t m_maxFrame = 0;

				// Depth
				VkImage m_depthImage = VK_NULL_HANDLE;
				VkImageView m_depthImageView = VK_NULL_HANDLE;
				VkDeviceMemory m_depthMemory = VK_NULL_HANDLE;
				VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;

				std::vector<VkSemaphore> m_imageAvailableSemaphores;
				std::vector<VkSemaphore> m_renderFinishedSemaphores;
				std::vector<VkFence> m_inFlightFences;

				ENGINE_API static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats);
				ENGINE_API static VkPresentModeKHR ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& a_availablePresentModes);
				ENGINE_API static VkExtent2D ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window);

				ENGINE_API static VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags, VkDevice a_device);

				std::vector<std::function<void(VkExtent2D a_extent)>> m_resizeCallback;
				void RecreateSwapChain(Window::IWindow* a_window, RHI::ILogicalDevice* a_logicalDevice, RHI::ISurface* a_surface, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::ICommandPool* a_commandPool);

			};
		}
	}
}

#endif