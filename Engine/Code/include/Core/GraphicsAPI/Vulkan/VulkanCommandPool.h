#ifndef VULKANCOMMANDPOOL_H
#define VULKANCOMMANDPOOL_H

#include <vulkan/vulkan.h>
#include <unordered_map>

#include "Core/Interfaces/ICommandPool.h"

namespace Engine
{
	namespace GamePlay
	{
		struct GameObjectData;
		class Camera;
	}

	namespace Core
	{
		namespace RHI
		{
			class IRenderObject;
			class IBuffer;
		}

		namespace GraphicsAPI
		{
			class VulkanLogicalDevice;
			class VulkanSwapchain;
			class VulkanGraphicPipeline;

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
				ENGINE_API static void RecordCommandBuffer(const VkCommandBuffer a_commandBuffer, const uint32_t a_imageIndex, const VkRenderPass a_renderPass, VulkanSwapchain* a_swapChain, const VulkanGraphicPipeline* a_graphicPipeline, const std::unordered_map<int, Core::RHI::IRenderObject*>& a_renderObjects, const std::vector<int>& a_ids, const std::unordered_map<int, Core::RHI::IBuffer*>& a_vertexBuffers, const std::unordered_map<int, Core::RHI::IBuffer*>& a_indexBuffers, const std::unordered_map<int, uint32_t>& a_nbIndices, const GamePlay::Camera* a_camera);

				ENGINE_API static VkCommandBuffer BeginSingleTimeCommands(VkDevice a_device, VkCommandPool a_commandPool);
				ENGINE_API static void EndSingleTimeCommands(VkCommandBuffer a_commandBuffer, VkCommandPool a_commandPool, VkDevice a_logicalDevice, VkQueue a_queue);

				std::vector<std::vector<std::tuple<VkCommandBuffer, VkRenderPass, VulkanGraphicPipeline*>>> mCommandBuffers{};
			private:
				VkCommandPool m_commandPool = VK_NULL_HANDLE;
			};
		}
	}
}

#endif