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

			struct RecordCommandBufferInfo
			{
				VkCommandBuffer a_commandBuffer;
				uint32_t a_imageIndex;
				VkRenderPass a_renderPass;
				VulkanSwapchain* a_swapChain;
				VulkanGraphicPipeline* a_graphicPipeline;
				GamePlay::Camera* a_camera;
			};

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
				ENGINE_API static void RecordCommandBuffer(RecordCommandBufferInfo info, const std::vector<Core::RHI::IRenderObject*>& a_renderObjects, const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers, const std::vector<Core::RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices);

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