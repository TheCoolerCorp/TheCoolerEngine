#ifndef ISWAPCHAIN_H
#define ISWAPCHAIN_H

#include "EngineExport.h"

#include <vector>

namespace Engine
{
	namespace GamePlay
	{
		struct GameObjectData;
		class Camera;
	}

	namespace Core
	{
		namespace Window
		{
			class IWindow;
		}
		namespace GraphicsAPI
		{
			class VulkanSwapchain;
		}

		namespace RHI
		{
			class ISurface;
			class IPhysicalDevice;
			class ILogicalDevice;
			class IRenderPass;
			class ICommandPool;
			class IGraphicPipeline;

			class ISwapChain
			{
			public:
				ENGINE_API virtual ~ISwapChain() = default;

				ENGINE_API virtual GraphicsAPI::VulkanSwapchain* CastVulkan() { return nullptr; }
				ENGINE_API virtual int GetMaxFrame() = 0;
				ENGINE_API virtual uint32_t GetCurrentFrame() = 0;

				ENGINE_API virtual void Create(ISurface* a_surface, Window::IWindow* a_window, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logical_device) = 0;
				ENGINE_API virtual void CreateFramebuffers(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::ICommandPool* a_commandPool, RHI::IGraphicPipeline* a_pipeline) = 0;
				ENGINE_API virtual void CreateSyncObjects(ILogicalDevice* a_logicalDevice) {}
				ENGINE_API virtual void Destroy(RHI::ILogicalDevice* a_logicalDevice) = 0;

				ENGINE_API virtual void DrawFrame(Window::IWindow* a_window, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool, RHI::ISurface* a_surface, RHI::IPhysicalDevice* a_physicalDevice, RHI::IRenderPass* a_renderPass, RHI::IGraphicPipeline* a_pipeline, std::vector<GamePlay::GameObjectData> a_objectsDatas, GamePlay::Camera* camera) = 0;

			};
		}
	}
}

#endif