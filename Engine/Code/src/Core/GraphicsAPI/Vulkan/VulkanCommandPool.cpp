#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

#include <array>

#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescritptor.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

#include "GamePlay/GameObject.h"

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

			void VulkanCommandPool::CreateCommandBuffer(RHI::ILogicalDevice* a_logicalDevice, RHI::ISwapChain* a_swapChain, RHI::IRenderPass* a_renderPass, RHI::IGraphicPipeline* a_graphicPipeline)
			{
				const uint32_t t_maxFrames = a_swapChain->CastVulkan()->GetMaxFrame();
				VkRenderPass t_renderPass = a_renderPass->CastVulkan()->GetRenderPass();
				VulkanGraphicPipeline* t_pipeline = a_graphicPipeline->CastVulkan();
				std::vector<std::tuple<VkCommandBuffer, VkRenderPass, VulkanGraphicPipeline*>> t_commandBuffers = std::vector<std::tuple<VkCommandBuffer, VkRenderPass, VulkanGraphicPipeline*>>(t_maxFrames);

				for (uint32_t i = 0; i < t_maxFrames; ++i)
				{
					VkCommandBuffer t_commandBuffer;
					VkCommandBufferAllocateInfo t_allocInfo{};
					t_allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
					t_allocInfo.commandPool = m_commandPool;
					t_allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
					t_allocInfo.commandBufferCount = 1;

					const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

					VK_CHECK(vkAllocateCommandBuffers(t_device, &t_allocInfo, &t_commandBuffer), "failed to allocate command buffers!");

					t_commandBuffers[i] = { t_commandBuffer, t_renderPass, t_pipeline };
				}

				mCommandBuffers.push_back(t_commandBuffers);
			}

			void VulkanCommandPool::RecordCommandBuffer(const VkCommandBuffer a_commandBuffer, const uint32_t a_imageIndex, const VkRenderPass a_renderPass, VulkanSwapchain* a_swapChain, const VulkanGraphicPipeline* a_graphicPipeline, GamePlay::GameObjectData* a_gameObjectDatas)
			{
				const VkExtent2D t_swapChainExtent = a_swapChain->GetExtent2D();
				const VkPipeline t_pipeline = a_graphicPipeline->GetPipeline();
				const VkPipelineLayout t_layout = a_graphicPipeline->GetLayout();

				VkCommandBufferBeginInfo t_beginInfo{};
				t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

				VK_CHECK(vkBeginCommandBuffer(a_commandBuffer, &t_beginInfo), "failed to begin recording command buffer!");

				VkRenderPassBeginInfo t_renderPassInfo{};
				t_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				t_renderPassInfo.renderPass = a_renderPass;
				t_renderPassInfo.framebuffer = a_swapChain->GetFramebuffers()[a_imageIndex];
				t_renderPassInfo.renderArea.offset = { .x= 0, .y= 0};
				t_renderPassInfo.renderArea.extent = t_swapChainExtent;

				std::array<VkClearValue, 2> t_clearValues{};
				t_clearValues[0].color = { {0.467f, 0.71f, 1.f, 0.996f} };
				t_clearValues[1].depthStencil = { .depth= 1.0f, .stencil= 0};

				t_renderPassInfo.clearValueCount = static_cast<uint32_t>(t_clearValues.size());
				t_renderPassInfo.pClearValues = t_clearValues.data();

				vkCmdBeginRenderPass(a_commandBuffer, &t_renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(a_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_pipeline);

				VkViewport t_viewport;
				t_viewport.x = 0.0f;
				t_viewport.y = 0.0f;
				t_viewport.width = static_cast<float>(a_swapChain->GetExtent2D().width);
				t_viewport.height = static_cast<float>(t_swapChainExtent.height);
				t_viewport.minDepth = 0.0f;
				t_viewport.maxDepth = 1.0f;
				vkCmdSetViewport(a_commandBuffer, 0, 1, &t_viewport);

				VkRect2D t_scissor;
				t_scissor.offset = { .x= 0, .y= 0};
				t_scissor.extent = t_swapChainExtent;
				vkCmdSetScissor(a_commandBuffer, 0, 1, &t_scissor);

				for (const GamePlay::GameObjectData t_gameObjectData : a_gameObjectDatas)
				{
					VkBuffer t_vertexBuffer = t_gameObjectData.mVertexBuffer->CastVulkan()->GetBuffer();
					constexpr VkDeviceSize t_offsets[] = { 0 };
					vkCmdBindVertexBuffers(a_commandBuffer, 0, 1, &t_vertexBuffer, t_offsets);

					vkCmdBindIndexBuffer(a_commandBuffer, t_gameObjectData.mIndexBuffer->CastVulkan()->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

					vkCmdBindDescriptorSets(a_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 1, 1,
					                        &t_gameObjectData.mDescriptor->CastVulkan()->GetDescriptorSets()[a_swapChain
						                        ->GetMaxFrame()], 0, nullptr);

					vkCmdDrawIndexed(a_commandBuffer, t_gameObjectData.mNbIndices, 1, 0, 0, 0);
				}

				vkCmdEndRenderPass(a_commandBuffer);

				VK_CHECK(vkEndCommandBuffer(a_commandBuffer), "failed to end command buffer!");
			}

			VkCommandBuffer VulkanCommandPool::BeginSingleTimeCommands(const VkDevice a_device, const VkCommandPool a_commandPool)
			{
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
			}
		}
	}
}
