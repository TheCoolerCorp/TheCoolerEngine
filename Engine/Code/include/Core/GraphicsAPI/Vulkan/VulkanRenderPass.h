#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include "EngineExport.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Interfaces/IRenderPass.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanRenderPass : public RHI::IRenderPass
			{
			public:
				ENGINE_API ~VulkanRenderPass() = default;
				ENGINE_API VulkanRenderPass* CastVulkan() { return this; }

				ENGINE_API void Create(RHI::ISwapChain* a_swapChain, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API void BeginRenderPass(RHI::ICommandPool* a_commandPool, RHI::ISwapChain* a_swapChain) override;
				ENGINE_API void EndRenderPass(RHI::ICommandPool* a_commandPool) override;


				ENGINE_API VkRenderPass GetRenderPass() { return m_renderPass; }
				ENGINE_API uint32_t GetSubpassesCount() { return m_subpasses; }
			private:
				VkRenderPass m_renderPass = VK_NULL_HANDLE;
				uint32_t m_subpasses = 0;
			};
		}
	}
}
#endif