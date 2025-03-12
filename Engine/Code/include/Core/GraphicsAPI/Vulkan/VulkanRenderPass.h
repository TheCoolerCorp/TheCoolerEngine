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
			class ENGINE_API VulkanRenderPass : public RHI::IRenderPass
			{
			public:
				~VulkanRenderPass() = default;
				GraphicsAPI::VulkanRenderPass* CastVulkan() { return this; }

				void Create(RHI::ISwapChain* a_swapChain, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice) override;
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				VkRenderPass GetRenderPass() { return m_renderPass; }
				uint32_t GetSubpassesCount() { return m_subpasses; }
			private:
				VkRenderPass m_renderPass = VK_NULL_HANDLE;
				uint32_t m_subpasses = 0;
			};
		}
	}
}
#endif