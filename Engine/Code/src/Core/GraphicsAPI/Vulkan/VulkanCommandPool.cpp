#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

#include <array>

#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescriptor.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

#include "GamePlay/Others/GameObject.h"
#include "GamePlay/ComponentsBase/Camera.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			VulkanCommandPool::~VulkanCommandPool()
			{
				for (auto& t_commandBuffer : mCommandBuffers)
				{
					t_commandBuffer.clear();
				}
				mCommandBuffers.clear();
			}

			void VulkanCommandPool::Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ILogicalDevice* a_logicalDevice)
			{
				const QueueFamilyIndices t_queueFamilyIndices = QueueFamilyIndices::FindQueueFamilies(a_physicalDevice->CastVulkan()->GetVkPhysicalDevice(), a_surface->CastVulkan()->GetVkSurfaceKHR());

				VkCommandPoolCreateInfo t_poolInfo{};
				t_poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				t_poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
				t_poolInfo.queueFamilyIndex = t_queueFamilyIndices.GetGraphicsFamily().value();

				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				VK_CHECK(vkCreateCommandPool(t_device, &t_poolInfo, nullptr, &m_commandPool), "failed to create command pool!");
			}

			void VulkanCommandPool::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				vkDestroyCommandPool(t_device, m_commandPool, nullptr);
			}

			void VulkanCommandPool::CreateCommandBuffer(RHI::ILogicalDevice* a_logicalDevice, uint32_t a_maxFrames)
			{
				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				std::vector<VkCommandBuffer> t_commandBuffers = std::vector<VkCommandBuffer>(a_maxFrames);
				for (uint32_t i = 0; i < a_maxFrames; ++i)
				{
					VkCommandBuffer t_commandBuffer;
					VkCommandBufferAllocateInfo t_allocInfo{};
					t_allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
					t_allocInfo.commandPool = m_commandPool;
					t_allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
					t_allocInfo.commandBufferCount = 1;

					VK_CHECK(vkAllocateCommandBuffers(t_device, &t_allocInfo, &t_commandBuffer), "failed to allocate command buffers!");

					t_commandBuffers[i] = t_commandBuffer;
				}
				mCommandBuffers.push_back(t_commandBuffers);
			}

			void VulkanCommandPool::BeginCommand(uint32_t a_commandBufferIndex, uint32_t a_currentFrame)
			{
				if (mCommandBuffers[a_commandBufferIndex].size() != 0)
				{
					VkCommandBuffer t_commandBuffer = mCommandBuffers[a_commandBufferIndex][a_currentFrame];
					VkCommandBufferBeginInfo t_beginInfo{};
					t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

					VK_CHECK(vkBeginCommandBuffer(t_commandBuffer, &t_beginInfo), "failed to begin recording command buffer!");
				}
			}

			void VulkanCommandPool::EndCommand(uint32_t a_commandBufferIndex, uint32_t a_currentFrame)
			{
				if (mCommandBuffers[a_commandBufferIndex].size() != 0)
				{
					VkCommandBuffer t_commandBuffer = mCommandBuffers[a_commandBufferIndex][a_currentFrame];

					VK_CHECK(vkEndCommandBuffer(t_commandBuffer), "failed to end recording command buffer!");
				}
			}

			void VulkanCommandPool::RecordCommandBuffer(const VkCommandBuffer a_commandBuffer, const uint32_t a_imageIndex, const VkRenderPass a_renderPass, VulkanSwapchain* a_swapChain, const VulkanGraphicPipeline* a_graphicPipeline, const std::vector<Core::RHI::IObjectDescriptor*>& a_renderObjects, const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers, const std::vector<Core::RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices, const GamePlay::Camera* a_camera)
			{
				//const VkExtent2D t_swapChainExtent = a_swapChain->GetExtent2D();
				//const VkPipeline t_pipeline = a_graphicPipeline->GetPipeline();
				//const VkPipelineLayout t_layout = a_graphicPipeline->GetLayout();

				//VkCommandBufferBeginInfo t_beginInfo{};
				//t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

				//VK_CHECK(vkBeginCommandBuffer(a_commandBuffer, &t_beginInfo), "failed to begin recording command buffer!");

				//VkRenderPassBeginInfo t_renderPassInfo{};
				//t_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				//t_renderPassInfo.renderPass = a_renderPass;
				//t_renderPassInfo.framebuffer = a_swapChain->GetFramebuffers()[a_imageIndex];
				//t_renderPassInfo.renderArea.offset = { .x = 0, .y = 0 };
				//t_renderPassInfo.renderArea.extent = t_swapChainExtent;

				//std::array<VkClearValue, 2> t_clearValues{};
				//t_clearValues[0].color = { {0.467f, 0.71f, 1.f, 0.996f} };
				//t_clearValues[1].depthStencil = { .depth = 1.0f, .stencil = 0 };

				//t_renderPassInfo.clearValueCount = static_cast<uint32_t>(t_clearValues.size());
				//t_renderPassInfo.pClearValues = t_clearValues.data(); 

				//vkCmdBeginRenderPass(a_commandBuffer, &t_renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				//vkCmdBindPipeline(a_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_pipeline);
				//
				//VkViewport t_viewport;
				//t_viewport.x = 0.0f;
				//t_viewport.y = 0.0f;
				//t_viewport.width = static_cast<float>(a_swapChain->GetExtent2D().width);
				//t_viewport.height = static_cast<float>(t_swapChainExtent.height);
				//t_viewport.minDepth = 0.0f;
				//t_viewport.maxDepth = 1.0f;
				//vkCmdSetViewport(a_commandBuffer, 0, 1, &t_viewport);

				//VkRect2D t_scissor;
				//t_scissor.offset = { .x = 0, .y = 0 };
				//t_scissor.extent = t_swapChainExtent;
				//vkCmdSetScissor(a_commandBuffer, 0, 1, &t_scissor);

				//// Change to common layout
				////const VkDescriptorSet t_cameraDescriptorSet = a_camera->GetDescriptor()->CastVulkan()->GetDescriptorSets(0);
				//vkCmdBindDescriptorSets(a_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 0, 1, &t_cameraDescriptorSet, 0, nullptr);

				//for (int i = 0; i < a_renderObjects.size(); ++i)
				//{
				//	if (a_vertexBuffers.at(i)->CastVulkan()->GetBuffer() != nullptr)
				//	{
				//		VkBuffer t_vertexBuffer = a_vertexBuffers.at(i)->CastVulkan()->GetBuffer();
				//		constexpr VkDeviceSize t_offsets[] = { 0 };
				//		vkCmdBindVertexBuffers(a_commandBuffer, 0, 1, &t_vertexBuffer, t_offsets);
				//	}

				//	if (a_indexBuffers.at(i)->CastVulkan()->GetBuffer() != nullptr)
				//	{
				//		vkCmdBindIndexBuffer(a_commandBuffer, a_indexBuffers.at(i)->CastVulkan()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
				//	}

				//	// Change to per layout
				//	vkCmdBindDescriptorSets(a_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 1, 1, &a_renderObjects.at(i)->CastVulkan()->GetDescriptorSets()[a_imageIndex], 0, nullptr);

				//	vkCmdDrawIndexed(a_commandBuffer, a_nbIndices.at(i), 1, 0, 0, 0);
				//}
				//vkCmdEndRenderPass(a_commandBuffer);

				//VK_CHECK(vkEndCommandBuffer(a_commandBuffer), "failed to end command buffer!");
			}

			VkCommandBuffer VulkanCommandPool::BeginSingleTimeCommands(const VkDevice a_device, const VkCommandPool a_commandPool)
			{
				m_commandsMutex.lock();
				VkCommandBufferAllocateInfo t_allocInfo{};
				t_allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				t_allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				t_allocInfo.commandPool = a_commandPool;
				t_allocInfo.commandBufferCount = 1;

				VkCommandBuffer t_commandBuffer;
				vkAllocateCommandBuffers(a_device, &t_allocInfo, &t_commandBuffer);

				VkCommandBufferBeginInfo t_beginInfo{};
				t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				t_beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

				vkBeginCommandBuffer(t_commandBuffer, &t_beginInfo);

				return t_commandBuffer;
			}

			void VulkanCommandPool::EndSingleTimeCommands(const VkCommandBuffer a_commandBuffer, const VkCommandPool a_commandPool, const VkDevice a_logicalDevice, const VkQueue a_queue)
			{
				vkEndCommandBuffer(a_commandBuffer);

				VkSubmitInfo t_submitInfo{};
				t_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				t_submitInfo.commandBufferCount = 1;
				t_submitInfo.pCommandBuffers = &a_commandBuffer;

				vkQueueSubmit(a_queue, 1, &t_submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(a_queue);

				vkFreeCommandBuffers(a_logicalDevice, a_commandPool, 1, &a_commandBuffer);
				m_commandsMutex.unlock();
			}
		}
	}
}
