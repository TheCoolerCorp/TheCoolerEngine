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

			class ENGINE_API VulkanCommandPool : public RHI::ICommandPool
			{
			public:
				void Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ILogicalDevice* a_logicalDevice) override;
				VkCommandPool GetVkCommandPool() const { return m_commandPool; }
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				VulkanCommandPool* CastVulkan() override { return this; }
				void CreateCommandBuffer(RHI::ILogicalDevice* a_logicalDevice) override;
				void RecordCommandBuffer(uint32_t a_imageIndex, RHI::IRenderPass* a_renderPass, RHI::ISwapChain* a_swapChain, RHI::IGraphicPipeline* a_graphicPipeline) override;

				static VkCommandBuffer BeginSingleTimeCommands(VkDevice a_device, VkCommandPool a_commandPool);
				static void EndSingleTimeCommands(VkCommandBuffer a_commandBuffer, VkCommandPool a_commandPool, VkDevice a_logicalDevice, VkQueue a_queue);

			private:
				VkCommandPool m_commandPool = VK_NULL_HANDLE;
				VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
			};
		}
	}
}

#endif