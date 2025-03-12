#ifndef VULKANINTERFACE_H
#define VULKANINTERFACE_H

#include "EngineExport.h"

#include "Core/Interfaces/ApiInterface.h"

#include "Core/Window/GLWindow.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanInterface : public RHI::ApiInterface
			{
			public:
				~VulkanInterface() override = default;

				Window::IWindow* InstantiateWindow() override { return new Window::GLwindow; }

				RHI::IInstance* InstantiateInstance() override { return new VulkanInstance; }

				RHI::IValidationLayers* InstantiateValidationLayers() override { return new VulkanValidationLayers; }

				RHI::ISurface* InstantiateSurface() override { return new VulkanSurface; }

				RHI::IPhysicalDevice* InstantiatePhysicalDevice() override { return new VulkanPhysicalDevice; }

				RHI::ILogicalDevice* InstantiateLogicalDevice() override { return new VulkanLogicalDevice; }

				RHI::ISwapChain* InstantiateSwapChain() override { return new VulkanSwapchain; }

				RHI::IRenderPass* InstantiateRenderPass() override { return new VulkanRenderPass; }

				RHI::IGraphicPipeline* InstantiateGraphicPipeline() override { return new VulkanGraphicPipeline; }

			};
		}
	}
}
#endif