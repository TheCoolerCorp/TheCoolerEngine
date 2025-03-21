#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "EngineExport.h"
#include "IImage.h"

#include "Core/Window/IWindow.h"
#include "Core/Interfaces/IInstance.h"
#include "Core/Interfaces/IValidationLayers.h"
#include "Core/Interfaces/ISurface.h"
#include "Core/Interfaces/IPhysicalDevice.h"
#include "Core/Interfaces/ILogicalDevice.h"
#include "Core/Interfaces/ISwapChain.h"
#include "Core/Interfaces/IRenderPass.h"
#include "Core/Interfaces/IGraphicPipeline.h"
#include "Core/Interfaces/ICommandPool.h"
#include "Core/Interfaces/IDescriptorPool.h"
#include "Core/Interfaces/IObjectDescriptor.h"
#include "Core/Interfaces/ICameraDescriptor.h"
#include "Core/Interfaces/IBuffer.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class ApiInterface
			{
			public:
				ENGINE_API virtual ~ApiInterface() = default;

				ENGINE_API virtual Window::IWindow* InstantiateWindow() = 0;
				ENGINE_API virtual void DestroyWindow(Window::IWindow* a_window) { delete a_window; }


				ENGINE_API virtual IInstance* InstantiateInstance() = 0;
				ENGINE_API virtual void DestroyInstance(IInstance* a_instance) { delete a_instance; }

				
				ENGINE_API virtual IValidationLayers* InstantiateValidationLayers() = 0;
				ENGINE_API virtual void DestroyValidationLayers(IValidationLayers* a_validationLayers) { delete a_validationLayers; }


				ENGINE_API virtual ISurface* InstantiateSurface() = 0;
				ENGINE_API virtual void DestroySurface(ISurface* a_surface) { delete a_surface; }


				ENGINE_API virtual IPhysicalDevice* InstantiatePhysicalDevice() = 0;
				ENGINE_API virtual void DestroyPhysicalDevice(IPhysicalDevice* a_physicalDevice) { delete a_physicalDevice; }


				ENGINE_API virtual ILogicalDevice* InstantiateLogicalDevice() = 0;
				ENGINE_API virtual void DestroyLogicalDevice(ILogicalDevice* a_logicalDevice) { delete a_logicalDevice; }


				ENGINE_API virtual ISwapChain* InstantiateSwapChain() = 0;
				ENGINE_API virtual void DestroySwapChain(ISwapChain* a_swapChain) { delete a_swapChain; }


				ENGINE_API virtual IRenderPass* InstantiateRenderPass() = 0;
				ENGINE_API virtual void DestroyRenderPass(IRenderPass* a_renderPass) { delete a_renderPass; }


				ENGINE_API virtual IGraphicPipeline* InstantiateGraphicPipeline() = 0;
				ENGINE_API virtual void DestroyGraphicPipeline(IGraphicPipeline* a_graphicPipeline) { delete a_graphicPipeline; }


				ENGINE_API virtual ICommandPool* InstantiateCommandPool() = 0;
				ENGINE_API virtual void DestroyCommandPool(ICommandPool* a_commandPool) { delete a_commandPool; }


				ENGINE_API virtual IDescriptorPool* InstantiateDescriptorPool() = 0;
				ENGINE_API virtual void DestroyDescriptorPool(IDescriptorPool* a_descriptorPool) { delete a_descriptorPool; }

				ENGINE_API virtual IObjectDescriptor* InstantiateObjectDescriptor() = 0;
				ENGINE_API virtual void DestroyObjectDescriptor(IObjectDescriptor* a_objectDescriptor) { delete a_objectDescriptor; }

				ENGINE_API virtual ICameraDescriptor* InstantiateCameraDescriptor() = 0;
				ENGINE_API virtual void DestroyCameraDescriptor(ICameraDescriptor* a_cameraDescriptor) { delete a_cameraDescriptor; }
				
				ENGINE_API virtual IBuffer* InstantiateBuffer() = 0;
				ENGINE_API virtual void DestroyBuffer(IBuffer* a_buffer) { delete a_buffer; }

				ENGINE_API virtual IImage* InstantiateImage() = 0;
				ENGINE_API virtual void DestroyImage(IImage* a_image) { delete a_image; }
			};
		}
	}
}

#endif