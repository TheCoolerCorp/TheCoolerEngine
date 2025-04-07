#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include <functional>
#include <map>
#include <utility>

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
			enum VulkanRenderPassFlags
			{
				FLAG_OVERRIDE_DEFAULT_RENDERPASS = 1
			};

			std::string VulkanRenderPassFlagToString(VulkanRenderPassFlags flag);

			class VulkanRenderPass : public RHI::IRenderPass
			{
			public:
				ENGINE_API ~VulkanRenderPass() = default;
				ENGINE_API VulkanRenderPass* CastVulkan() { return this; }

				ENGINE_API void Create(RHI::ISwapChain* a_swapChain, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VkRenderPass GetRenderPass() { return m_renderPass; }
				ENGINE_API uint32_t GetSubpassesCount() { return m_subpasses; }

				//default scene renderpass
				ENGINE_API void RecordSceneRenderPass(VkRecordCommandBufferInfo info, std::vector<GamePlay::GameObjectData> objectsData, std::vector<VkCommandBuffer>& buffers);

				ENGINE_API static void AddRenderPass(std::function<void(Engine::Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>, std::vector<VkCommandBuffer>&)> callback, int priority = 1)
				{
					if (m_renderPasses[priority].empty())
						m_renderPassCount++;
					m_renderPasses[priority].push_back(callback);
				}

				ENGINE_API static void RunRenderPass(VkRecordCommandBufferInfo info, std::vector<GamePlay::GameObjectData> data, std::vector<VkCommandBuffer>& buffers)
				{
					if (m_currentRenderCallback == 0)
						NextRenderPass();
					for (auto& callback : m_renderPasses[m_currentRenderCallback])
					{
						callback(info, data, buffers);
					}
				}

				ENGINE_API static void NextRenderPass()
				{
					auto it = m_renderPasses.upper_bound(m_currentRenderCallback);
					if (it != m_renderPasses.end())
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
					if (m_renderPasses[priority].empty() && priority != 0)
						LOG_WARNING("VulkanRenderPass: Added callback to priority with no associated renderpasses, callback will not be called");
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

				ENGINE_API static void SetSceneRenderPass(std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>,std::vector<VkCommandBuffer>&)> callback)
				{
					m_sceneRenderPass = std::move(callback);
				}

				ENGINE_API static void RunSceneRenderPass(Core::GraphicsAPI::VkRecordCommandBufferInfo info, std::vector<GamePlay::GameObjectData> data, std::vector<VkCommandBuffer>& buffers)
				{
					if (m_sceneRenderPass)
					{
						m_sceneRenderPass(info, data, buffers);
					}
				}

				//add renderpass flag if it doesn't exist
				ENGINE_API static void AddRenderPassFlag(VulkanRenderPassFlags flag)
				{
					if(std::ranges::find(m_renderPassFlags, flag) == m_renderPassFlags.end())
						m_renderPassFlags.push_back(flag);
				}

				ENGINE_API static void RemoveRenderPassFlag(VulkanRenderPassFlags flag)
				{
					if (auto it = std::ranges::find(m_renderPassFlags, flag); it != m_renderPassFlags.end())
						m_renderPassFlags.erase(it);
					else
						LOG_WARNING("Tried to remove vulkan render pass flag " + VulkanRenderPassFlagToString(flag) + ", but flag is not present");
				}

				ENGINE_API static bool HasRenderPassFlag(VulkanRenderPassFlags flag)
				{
					return std::ranges::find(m_renderPassFlags, flag) != m_renderPassFlags.end();
				}
			private:
				/*static function because we want to call it quickly and from everywhere
			 * this is an important callback for any render-related functions like ImGUI and i do not want it hidden
			 * in something like the VulkanCommandPool that will actually call it, because the call chain to
			 * set it there is long and inconvenient. As such i think a static for such an important function is justified
			 */
				static std::map<int,std::vector<std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>, std::vector<VkCommandBuffer>&)>>> m_renderPasses;
				//callbacks that get executed after a specified render pass id
				static std::map<int, std::vector<std::function<void()>>> m_renderPassCallbacks;
				static std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>, std::vector<VkCommandBuffer>&)> m_sceneRenderPass;
				static int m_renderPassCount;
				static int m_currentRenderCallback;

				static std::vector<VulkanRenderPassFlags> m_renderPassFlags;

				VkRenderPass m_renderPass = VK_NULL_HANDLE;
				uint32_t m_subpasses = 0;
			};
		}
	}
}
#endif