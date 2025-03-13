#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Interfaces/IRenderPass.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
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

			void VulkanCommandPool::CreateCommandBuffer(RHI::ILogicalDevice* a_logicalDevice)
			{
				VkCommandBufferAllocateInfo t_allocInfo{};
				t_allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				t_allocInfo.commandPool = m_commandPool;
				t_allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				t_allocInfo.commandBufferCount = 1;

				const VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				VK_CHECK(vkAllocateCommandBuffers(t_device, &t_allocInfo, &m_commandBuffer), "failed to allocate command buffers!");
			}

			void VulkanCommandPool::RecordCommandBuffer(uint32_t a_imageIndex, RHI::IRenderPass* a_renderPass)
			{
				VkRenderPass t_renderPass = a_renderPass->CastVulkan()->GetRenderPass();

				VkCommandBufferBeginInfo t_beginInfo{};
				t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				t_beginInfo.flags = 0;
				t_beginInfo.pInheritanceInfo = nullptr;

				VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &t_beginInfo), "failed to begin recording command buffer!");

				VkRenderPassBeginInfo t_renderPassInfo{};
				t_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				t_renderPassInfo.renderPass = t_renderPass;
				//t_renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
			}
		}
	}
}
