#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include <functional>

#include "EngineExport.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Interfaces/IRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

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

				ENGINE_API VkRenderPass GetRenderPass() { return m_renderPass; }
				ENGINE_API uint32_t GetSubpassesCount() { return m_subpasses; }

				ENGINE_API static void AddRenderCallBack(std::function<void(VkCommandBuffer)> callback)
				{
					m_renderCallbacks.push_back(callback);
				}

				ENGINE_API static void RunRenderCallbacks(VkCommandBuffer commandBuffer)
				{
					for (auto& callback : m_renderCallbacks)
					{
						callback(commandBuffer);
					}
				}

				ENGINE_API static void AddRenderPass(std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo)> callback)
				{
					m_renderPasses.push_back(callback);
				}

				ENGINE_API static void RunRenderPass(VkRecordCommandBufferInfo info)
				{
					auto& callback = m_renderPasses[currentRenderCallback];
					callback(info);
				}

				ENGINE_API static void NextRenderPass()
				{
					currentRenderCallback++;
				}
			private:
				/*static function because we want to call it quickly and from everywhere
			 * this is an important callback for any render-related functions like ImGUI and i do not want it hidden
			 * in something like the VulkanCommandPool that will actually call it, because the call chain to
			 * set it there is long and inconvenient. As such i think a static for such an important function is justified
			 */
				static std::vector<std::function<void(VkCommandBuffer)>> m_renderCallbacks;
				static std::vector<std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo)>> m_renderPasses;
				static int currentRenderCallback;


				VkRenderPass m_renderPass = VK_NULL_HANDLE;
				uint32_t m_subpasses = 0;
			};
		}
	}
}
#endif