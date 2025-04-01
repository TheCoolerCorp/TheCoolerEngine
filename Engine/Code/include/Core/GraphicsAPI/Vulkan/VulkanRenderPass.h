#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include <functional>
#include <map>

#include "EngineExport.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Interfaces/IRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "GamePlay/GameObject.h"

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

				ENGINE_API static void AddRenderPass(std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>)> callback, int priority = 1)
				{
					if (m_renderPasses[priority].empty())
						m_renderPassCount++;
					m_renderPasses[priority].push_back(callback);
				}

				ENGINE_API static void RunRenderPass(VkRecordCommandBufferInfo info, std::vector<GamePlay::GameObjectData> data)
				{
					for (auto& callback : m_renderPasses[m_currentRenderCallback])
					{
						callback(info, data);
					}
				}

				ENGINE_API static void NextRenderPass()
				{
					auto it = m_renderPassCallbacks.upper_bound(m_currentRenderCallback);
					if (it != m_renderPassCallbacks.end())
					{
						m_currentRenderCallback = it->first;
					}
				}

				ENGINE_API static void ResetRenderPassCount()
				{
					m_currentRenderCallback = 0;
				}

				ENGINE_API static void AddRenderPassCallback(std::function<void()> callback, int priority)
				{
					m_renderPassCallbacks[priority].push_back(callback);
				}

				ENGINE_API static void RunRenderPassCallbacks()
				{
					for (auto& callback : m_renderPassCallbacks[m_currentRenderCallback])
					{
						callback();
					}
				}

				ENGINE_API static int GetRenderPassCount() { return m_renderPassCount; }

			private:
				/*static function because we want to call it quickly and from everywhere
			 * this is an important callback for any render-related functions like ImGUI and i do not want it hidden
			 * in something like the VulkanCommandPool that will actually call it, because the call chain to
			 * set it there is long and inconvenient. As such i think a static for such an important function is justified
			 */
				static std::map<int,std::vector<std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>)>>> m_renderPasses;
				//callbacks that get executed after a specified render pass id
				static std::map<int, std::vector<std::function<void()>>> m_renderPassCallbacks;
				static int m_renderPassCount;
				static int m_currentRenderCallback;


				VkRenderPass m_renderPass = VK_NULL_HANDLE;
				uint32_t m_subpasses = 0;
			};
		}
	}
}
#endif