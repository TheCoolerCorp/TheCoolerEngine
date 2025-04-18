#ifndef VULKANINTERFACE_H
#define VULKANINTERFACE_H

#include "EngineExport.h"
#include "VulkanBuffer.h"

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
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanImage.h"
#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescriptor.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanShader.h"
namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanInterface : public RHI::ApiInterface
			{
			public:
				ENGINE_API ~VulkanInterface() override = default;

				ENGINE_API Window::IWindow* InstantiateWindow() override { return new Window::GLwindow; }

				ENGINE_API RHI::IInstance* InstantiateInstance() override { return new VulkanInstance; }

				ENGINE_API RHI::IValidationLayers* InstantiateValidationLayers() override { return new VulkanValidationLayers; }

				ENGINE_API RHI::ISurface* InstantiateSurface() override { return new VulkanSurface; }

				ENGINE_API RHI::IPhysicalDevice* InstantiatePhysicalDevice() override { return new VulkanPhysicalDevice; }

				ENGINE_API RHI::ILogicalDevice* InstantiateLogicalDevice() override { return new VulkanLogicalDevice; }

				ENGINE_API RHI::ISwapChain* InstantiateSwapChain() override { return new VulkanSwapchain; }

				ENGINE_API RHI::IRenderPass* InstantiateRenderPass() override { return new VulkanRenderPass; }

				ENGINE_API RHI::IGraphicPipeline* InstantiateGraphicPipeline() override { return new VulkanGraphicPipeline; }

				ENGINE_API RHI::ICommandPool* InstantiateCommandPool() override { return new VulkanCommandPool; }

				ENGINE_API RHI::IDescriptorPool* InstantiateDescriptorPool() override { return new VulkanDescriptorPool; }

				ENGINE_API RHI::IBuffer* InstantiateBuffer() override { return new VulkanBuffer; }

				ENGINE_API RHI::IImage* InstantiateImage() override { return new VulkanImage; }

				ENGINE_API RHI::IObjectDescriptor* InstantiateObjectDescriptor() override { return new VulkanObjectDescriptor; }

				ENGINE_API RHI::IShader* InstantiateShader() override { return new VulkanShader; }
			};
		}
	}
}
#endif