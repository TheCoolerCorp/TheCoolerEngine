#ifndef VULKANCOMMANDPOOL_H
#define VULKANCOMMANDPOOL_H

#include <vulkan/vulkan.h>

#include "Core/Interfaces/ICommandPool.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanLogicalDevice;
			class VulkanSwapchain;

			class VulkanCommandPool : public RHI::ICommandPool
			{
			public:
				ENGINE_API VulkanCommandPool() = default;
				ENGINE_API ~VulkanCommandPool() override;
				ENGINE_API void Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API VkCommandPool GetVkCommandPool() const { return m_commandPool; }
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API VulkanCommandPool* CastVulkan() override { return this; }
				ENGINE_API void CreateCommandBuffer(RHI::ILogicalDevice* a_logicalDevice, RHI::ISwapChain* a_swapChain, RHI::IRenderPass* a_renderPass, RHI::IGraphicPipeline* a_graphicPipeline) override;
				ENGINE_API static void RecordCommandBuffer(const VkCommandBuffer a_commandBuffer, const uint32_t a_imageIndex, const VkRenderPass a_renderPass, const VulkanSwapchain* a_swapChain, const VkPipeline a_graphicPipeline);

				ENGINE_API static VkCommandBuffer BeginSingleTimeCommands(VkDevice a_device, VkCommandPool a_commandPool);
				ENGINE_API static void EndSingleTimeCommands(VkCommandBuffer a_commandBuffer, VkCommandPool a_commandPool, VkDevice a_logicalDevice, VkQueue a_queue);

				std::vector<std::vector<std::tuple<VkCommandBuffer, VkRenderPass, VkPipeline>>> mCommandBuffers{};
			private:
				VkCommandPool m_commandPool = VK_NULL_HANDLE;
			};
		}
	}
}

#endif