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
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/Interfaces/IGraphicPipeline.h"
#include "GamePlay/GameObject.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			VulkanSwapchain::VulkanSwapchain() : m_swapChainImageFormat(), m_swapChainExtent(),
			                                     m_imageIndex(0){}

			/*
			 * Creates a swapchain and all elements related to it
			 */
			void VulkanSwapchain::Create(RHI::ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice)
			{
				uint32_t t_imageCount = 0;
				const VulkanSurface* t_surface = a_surface->CastVulkan();
				const VkSurfaceCapabilitiesKHR t_surfaceCapabilities = t_surface->GetSurfaceCapabilities();
				const std::vector<VkSurfaceFormatKHR> t_surfaceFormats = t_surface->GetSurfaceFormats();
				const std::vector<VkPresentModeKHR> t_surfacePresentModes = t_surface->GetSurfacePresentModes();
				if (m_maxFrame <= 0)
				{
					t_imageCount = t_surfaceCapabilities.minImageCount + 1;

					if (t_surfaceCapabilities.maxImageCount > 0 && t_imageCount > t_surfaceCapabilities.maxImageCount) 
					{
						t_imageCount = t_surfaceCapabilities.maxImageCount;
					}
				}
				else
				{
					t_imageCount = m_maxFrame;
				}

				const VkSurfaceFormatKHR t_formats = ChooseSurfaceFormat(t_surfaceFormats);
				const VkPresentModeKHR t_presentMode = ChooseSurfacePresentMode(t_surfacePresentModes);
				const VkExtent2D t_extent = ChooseSurfaceExtent(t_surfaceCapabilities, a_window);


				VkSwapchainCreateInfoKHR t_createInfo = {};
				t_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

				t_createInfo.surface = a_surface->CastVulkan()->GetVkSurfaceKHR();
				t_createInfo.minImageCount = t_imageCount;
				t_createInfo.imageFormat = t_formats.format;
				t_createInfo.imageColorSpace = t_formats.colorSpace;
				t_createInfo.imageExtent = t_extent;
				t_createInfo.imageArrayLayers = 1;
				t_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

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

				t_createInfo.preTransform = t_surfaceCapabilities.currentTransform;
				t_createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				t_createInfo.presentMode = t_presentMode;
				t_createInfo.clipped = VK_TRUE;

				t_createInfo.oldSwapchain = VK_NULL_HANDLE;

				VK_CHECK(vkCreateSwapchainKHR(a_logicalDevice->CastVulkan()->GetVkDevice(), &t_createInfo, nullptr, &m_swapChain), "failed to create swap chain!");

				vkGetSwapchainImagesKHR(a_logicalDevice->CastVulkan()->GetVkDevice(), m_swapChain, &t_imageCount, nullptr);
				m_images.resize(t_imageCount);
				m_imageViews.resize(t_imageCount);
				vkGetSwapchainImagesKHR(a_logicalDevice->CastVulkan()->GetVkDevice(), m_swapChain, &t_imageCount, m_images.data());

				m_swapChainImageFormat = t_formats.format;
				m_swapChainExtent = t_extent;
				m_maxFrame = t_imageCount;

				// Create imagesViews;
				for (int i = 0; i < m_images.size(); ++i)
				{
					VulkanImage::CreateImageView(m_images[i], &m_imageViews[i], a_logicalDevice->CastVulkan()->GetVkDevice(), m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);


					m_depthFormat = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
																									VK_IMAGE_TILING_OPTIMAL,
																									VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
				}

			}

/*
Creates framebuffers and associated resources for the Vulkan swapchain. 
This includes depth buffer creation, image views, descriptor sets, samplers, 
and framebuffers for each swapchain image. Ensures resources are properly allocated 
and configured for rendering.
 */
			void VulkanSwapchain::CreateFramebuffers(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::ICommandPool* a_commandPool, RHI::IGraphicPipeline* a_pipeline)
			{
				VkDescriptorSetLayout t_descriptorSetLayout = a_pipeline->CastVulkan()->GetObjectDescriptorSetLayout();
				m_descriptorPool = new VulkanDescriptorPool();
				m_descriptorPool->Create(a_logicalDevice, static_cast<int>(m_imageViews.size()));
				VkFormat t_depthFormat = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
				VulkanImage::CreateImage(&m_depthImage, &m_depthMemory, a_logicalDevice->CastVulkan()->GetVkDevice(),
				                         a_physicalDevice->CastVulkan()->GetVkPhysicalDevice(), m_swapChainExtent.width,
				                         m_swapChainExtent.height, m_depthFormat, VK_IMAGE_TILING_OPTIMAL,
				                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				VulkanImage::CreateImageView(m_depthImage, &m_depthImageView,
				                             a_logicalDevice->CastVulkan()->GetVkDevice(), m_depthFormat,
				                             VK_IMAGE_ASPECT_DEPTH_BIT);
				VulkanImage::TransitionImageLayout(m_depthImage, a_logicalDevice->CastVulkan()->GetVkDevice(),
				                                   a_logicalDevice->CastVulkan()->GetGraphicsQueue(),
				                                   a_commandPool->CastVulkan()->GetVkCommandPool(), t_depthFormat,
				                                   VK_IMAGE_LAYOUT_UNDEFINED,
				                                   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

				m_framebuffers.resize(m_imageViews.size());
				m_samplers.resize(m_imageViews.size());
				m_descriptorSets.resize(m_imageViews.size());

				const std::vector<VkDescriptorSetLayout> t_layouts(m_imageViews.size(), t_descriptorSetLayout);
				VkDescriptorSetAllocateInfo t_allocInfo{};
				t_allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				t_allocInfo.descriptorPool = m_descriptorPool->GetPool();
				t_allocInfo.descriptorSetCount = static_cast<uint32_t>(m_imageViews.size());
				t_allocInfo.pSetLayouts = t_layouts.data();

				VK_CHECK(vkAllocateDescriptorSets(a_logicalDevice->CastVulkan()->GetVkDevice(), &t_allocInfo, m_descriptorSets.data()), "Can't allocate descriptor sets");

				for (size_t i = 0; i < m_framebuffers.size(); i++)
				{
					VulkanImage::CreateSampler(&m_samplers[i], a_logicalDevice->CastVulkan()->GetVkDevice(), a_physicalDevice->CastVulkan()->GetVkPhysicalDevice());

					VkDescriptorImageInfo t_imageInfo{};
					t_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					t_imageInfo.imageView = m_imageViews[i];
					t_imageInfo.sampler = m_samplers[i];

					std::array<VkWriteDescriptorSet, 1> t_descriptorWrites{};

					t_descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					t_descriptorWrites[0].dstSet = m_descriptorSets[i];
					t_descriptorWrites[0].dstBinding = 1;
					t_descriptorWrites[0].dstArrayElement = 0;
					t_descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					t_descriptorWrites[0].descriptorCount = 1;
					t_descriptorWrites[0].pImageInfo = &t_imageInfo;

					vkUpdateDescriptorSets(a_logicalDevice->CastVulkan()->GetVkDevice(), static_cast<uint32_t>(t_descriptorWrites.size()), t_descriptorWrites.data(), 0, nullptr);

					std::vector<VkImageView> attachments;

					attachments.push_back(m_imageViews[i]);
					attachments.push_back(m_depthImageView);

					VkFramebufferCreateInfo framebufferInfo{};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = a_renderPass->CastVulkan()->GetRenderPass();
					framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = m_swapChainExtent.width;
					framebufferInfo.height = m_swapChainExtent.height;
					framebufferInfo.layers = 1;

					VK_CHECK(vkCreateFramebuffer(a_logicalDevice->CastVulkan()->GetVkDevice(), &framebufferInfo, nullptr, &m_framebuffers[i]), "Failed to create framebuffers in swapchain");
				}
			}

			void VulkanSwapchain::CreateSyncObjects(RHI::ILogicalDevice* a_logicalDevice)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				m_imageAvailableSemaphores.resize(m_maxFrame);
				m_renderFinishedSemaphores.resize(m_maxFrame);
				m_inFlightFences.resize(m_maxFrame);

				VkSemaphoreCreateInfo t_semaphoreInfo{};
				t_semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

				VkFenceCreateInfo t_fenceInfo{};
				t_fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				t_fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

				for (size_t i = 0; i < m_maxFrame; i++) {
					VK_CHECK(vkCreateSemaphore(t_device, &t_semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]), "failed to create synchronization objects for a frame!")
					VK_CHECK(vkCreateSemaphore(t_device, &t_semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]), "failed to create synchronization objects for a frame!")
					VK_CHECK(vkCreateFence(t_device, &t_fenceInfo, nullptr, &m_inFlightFences[i]), "failed to create synchronization objects for a frame!")
				}

			}

			std::vector<VkFramebuffer> VulkanSwapchain::GetFramebuffers() const
			{
				return m_framebuffers;
			}

/*
 Handles rendering a single frame in Vulkan.
 This includes acquiring an image from the swapchain, recording command buffers,
 submitting them for execution, and presenting the rendered image.
 Also manages synchronization using semaphores and fences,
 and recreates the swapchain if needed.
 */
			void VulkanSwapchain::DrawFrame(Window::IWindow* a_window, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool, RHI::ISurface* a_surface, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::IGraphicPipeline* a_pipeline, std::vector<GamePlay::GameObjectData> a_objectsData, GamePlay::Camera* camera)
			{
				VulkanRenderPass::ResetRenderPassCount();
				VulkanCommandPool* t_commandPool = a_commandPool->CastVulkan();
				const VulkanLogicalDevice* t_logicalDevice = a_logicalDevice->CastVulkan();
				const VkDevice t_device = t_logicalDevice->GetVkDevice();

				// Wait for the previous frame to finish before proceeding
				vkWaitForFences(t_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

				uint32_t t_imageIndex;
				// Acquire an image from the swapchain for rendering
				VkResult t_result = vkAcquireNextImageKHR(t_device, m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &t_imageIndex);

				// If the swapchain is outdated (e.g., due to resizing), recreate it and exit early
				if (t_result == VK_ERROR_OUT_OF_DATE_KHR) {
					RecreateSwapChain(a_window, a_logicalDevice, a_surface, a_physicalDevice, a_renderPass, a_commandPool, a_pipeline);
					return;
				}
				if (t_result != VK_SUCCESS && t_result != VK_SUBOPTIMAL_KHR) {
					throw std::runtime_error("failed to acquire swap chain image!");
				}

				// Reset the fence so we can wait for this frame's completion later
				vkResetFences(t_device, 1, &m_inFlightFences[m_currentFrame]);

				const VkSemaphore t_signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };

				for (int a = 0; a<VulkanRenderPass::GetRenderPassCount(); a++)
				{
					
						

					// Record command buffers for rendering
					std::vector<VkCommandBuffer> t_commandBuffers;
					for (int i = 0; i < static_cast<int>(t_commandPool->mCommandBuffers.size()); ++i)
					{
						const VkCommandBuffer t_commandBuffer = std::get<VkCommandBuffer>(t_commandPool->mCommandBuffers[i][m_currentFrame]);
						const VkRenderPass t_renderPass = std::get<VkRenderPass>(t_commandPool->mCommandBuffers[i][m_currentFrame]);
						const VulkanGraphicPipeline* t_pipeline = std::get<VulkanGraphicPipeline*>(t_commandPool->mCommandBuffers[i][m_currentFrame]);

						t_commandBuffers.push_back(t_commandBuffer);
						vkResetCommandBuffer(t_commandBuffer, 0);

						VkRecordCommandBufferInfo info;
						info.commandBuffer = t_commandBuffer;
						info.imageIndex = t_imageIndex;
						info.renderPass = t_renderPass;
						info.swapChain = this;
						info.graphicPipeline = t_pipeline;
						info.camera = camera;

						//RecordCommandBuffer calls RunRenderPass for the current renderpass
						VulkanCommandPool::RecordCommandBuffer(info, a_objectsData);
					}


					// Set up the submit info struct for queue submission
					VkSubmitInfo t_submitInfo{};
					t_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

					const VkSemaphore t_waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
					constexpr VkPipelineStageFlags t_waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
					t_submitInfo.waitSemaphoreCount = 1;
					t_submitInfo.pWaitSemaphores = t_waitSemaphores;
					t_submitInfo.pWaitDstStageMask = t_waitStages;

					t_submitInfo.commandBufferCount = static_cast<uint32_t>(t_commandPool->mCommandBuffers.size());
					t_submitInfo.pCommandBuffers = t_commandBuffers.data();
					
					t_submitInfo.signalSemaphoreCount = 1;
					t_submitInfo.pSignalSemaphores = t_signalSemaphores;

					// Submit the command buffer for execution on the graphics queue
					VK_CHECK(vkQueueSubmit(t_logicalDevice->GetGraphicsQueue(), 1, &t_submitInfo, m_inFlightFences[m_currentFrame]), "failed to submit draw command buffer!");

					//runs the post-renderpass callbacks and advances to the next renderpass
					VulkanRenderPass::RunRenderPassCallbacks();
					VulkanRenderPass::NextRenderPass();
					t_commandBuffers.clear();
				}
				


				// Set up the present info struct for presenting the rendered frame
				VkPresentInfoKHR t_presentInfo{};
				t_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

				t_presentInfo.waitSemaphoreCount = 1;
				t_presentInfo.pWaitSemaphores = t_signalSemaphores;

				const VkSwapchainKHR t_swapChains[] = { m_swapChain };
				t_presentInfo.swapchainCount = 1;
				t_presentInfo.pSwapchains = t_swapChains;

				t_presentInfo.pImageIndices = &t_imageIndex;

				// Present the image to the swapchain
				t_result = vkQueuePresentKHR(t_logicalDevice->GetPresentQueue(), &t_presentInfo);

				// Handle swapchain recreation if the window was resized
				if (t_result == VK_ERROR_OUT_OF_DATE_KHR || t_result == VK_SUBOPTIMAL_KHR || a_window->GetResized()) {
					a_window->SetResized(false);
					RecreateSwapChain(a_window, a_logicalDevice, a_surface, a_physicalDevice, a_renderPass, a_commandPool, a_pipeline);
				}
				else if (t_result != VK_SUCCESS) {
					throw std::runtime_error("failed to present swap chain image!");
				}

				// Advance to the next frame in the swapchain
				m_currentFrame = (m_currentFrame + 1) % m_maxFrame;
			}

			void VulkanSwapchain::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				CleanupSwapChain(a_logicalDevice);

				for (size_t i = 0; i < m_maxFrame; i++) {
					vkDestroySemaphore(t_device, m_renderFinishedSemaphores[i], nullptr);
					vkDestroySemaphore(t_device, m_imageAvailableSemaphores[i], nullptr);
					vkDestroyFence(t_device, m_inFlightFences[i], nullptr);
				}
				m_renderFinishedSemaphores.clear();
				m_imageAvailableSemaphores.clear();
				m_inFlightFences.clear();
			}

			/*
			 * Cleans up all vulkan swapchain-related ressources
			 */
			void VulkanSwapchain::CleanupSwapChain(RHI::ILogicalDevice* a_logicalDevice)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				for (const auto& t_framebuffer : m_framebuffers)
				{
					vkDestroyFramebuffer(t_device, t_framebuffer, nullptr);
				}

				for (const auto& t_imageView : m_imageViews)
				{
					vkDestroyImageView(t_device, t_imageView, nullptr);
				}

				for (const auto& t_sampler : m_samplers)
				{
					vkDestroySampler(t_device, t_sampler, nullptr);
				}

				vkFreeDescriptorSets(t_device, m_descriptorPool->GetPool(), static_cast<uint32_t>(m_descriptorSets.size()), m_descriptorSets.data());
				m_descriptorSets.clear();

				m_samplers.clear();
				vkDestroyImage(t_device, m_depthImage, nullptr);
				vkDestroyImageView(t_device, m_depthImageView, nullptr);
				vkFreeMemory(t_device, m_depthMemory, nullptr);
				vkDestroySwapchainKHR(t_device, m_swapChain, nullptr);
				m_descriptorPool->Destroy(a_logicalDevice);
				delete m_descriptorPool;
			}

			VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& a_availableFormats)
			{
				ASSERT(!a_availableFormats.empty(), "No available formats !");
				for (const auto& t_availableFormat : a_availableFormats)
				{
					if (t_availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && t_availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
					{
						return t_availableFormat;
					}
				}

				return a_availableFormats[0];
			}

			VkPresentModeKHR VulkanSwapchain::ChooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& a_availablePresentModes)
			{
				return VK_PRESENT_MODE_FIFO_KHR; // TODO : clean this
				for (const auto& t_availablePresentMode : a_availablePresentModes)
				{
					if (t_availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
					{
						return t_availablePresentMode;
					}
				}
			}


			VkExtent2D VulkanSwapchain::ChooseSurfaceExtent(const VkSurfaceCapabilitiesKHR& a_availableCapabilities, Window::IWindow* a_window)
			{
				if (a_availableCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
				{
					return a_availableCapabilities.currentExtent;
				}
				int t_width, t_height = 0;
				a_window->CastGLFW()->GetFramebufferSize(&t_width, &t_height);

				VkExtent2D t_currentExtent = 
				{
					static_cast<uint32_t>(t_width),
					static_cast<uint32_t>(t_height)
				};

				t_currentExtent.width = std::clamp(t_currentExtent.width, a_availableCapabilities.minImageExtent.width, a_availableCapabilities.maxImageExtent.width);
				t_currentExtent.height = std::clamp(t_currentExtent.height, a_availableCapabilities.minImageExtent.height, a_availableCapabilities.maxImageExtent.height);

				return t_currentExtent;
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

/*
 Recreates the vulkan swapchain https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Swap_chain
 waits for the window to not be minimized then waits for the device to be idle
 Cleans up the old swapchain and creates a new one along with the framebuffers
 */
			void VulkanSwapchain::RecreateSwapChain(Window::IWindow* a_window, RHI::ILogicalDevice* a_logicalDevice,
			                                        RHI::ISurface* a_surface, RHI::IPhysicalDevice* a_physicalDevice,
			                                        RHI::IRenderPass* a_renderPass, RHI::ICommandPool* a_commandPool,
													RHI::IGraphicPipeline* a_pipeline)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				//Wait till the framebuffer is valid
				a_window->ResizeFramebuffer();

				//wait till the device is idle
				vkDeviceWaitIdle(t_device);

				//clean up the old swapchain
				CleanupSwapChain(a_logicalDevice);

				//create a new swapchain
				Create(a_surface, a_window, a_physicalDevice, a_logicalDevice);
				//create framebuffers
				CreateFramebuffers(a_logicalDevice, a_physicalDevice, a_renderPass, a_commandPool, a_pipeline);
			}
		}
	}
}
