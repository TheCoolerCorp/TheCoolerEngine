#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "EngineExport.h"

#include "Core/Window/IWindow.h"
#include "Core/Interfaces/IInstance.h"
#include "Core/Interfaces/IValidationLayers.h"
#include "Core/Interfaces/ISurface.h"
#include "Core/Interfaces/IPhysicalDevice.h"
#include "Core/Interfaces/ILogicalDevice.h"
#include "Core/Interfaces/ISwapChain.h"
#include "Core/Interfaces/IRenderPass.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class ENGINE_API ApiInterface
			{
			public:
				virtual ~ApiInterface() = default;

				virtual Window::IWindow* InstantiateWindow() = 0;
				virtual void DestroyWindow(Window::IWindow* a_window) { delete a_window; }
				
				virtual IInstance* InstantiateInstance() = 0;
				virtual void DestroyInstance(IInstance* a_instance) { delete a_instance; }

				
				virtual IValidationLayers* InstantiateValidationLayers() = 0;
				virtual void DestroyValidationLayers(IValidationLayers* a_validationLayers) { delete a_validationLayers; }

				virtual ISurface* InstantiateSurface() = 0;
				virtual void DestroySurface(ISurface* a_surface) { delete a_surface; }

				virtual IPhysicalDevice* InstantiatePhysicalDevice() = 0;
				virtual void DestroyPhysicalDevice(IPhysicalDevice* a_physicalDevice) { delete a_physicalDevice; }

				virtual ILogicalDevice* InstantiateLogicalDevice() = 0;
				virtual void DestroyLogicalDevice(ILogicalDevice* a_logicalDevice) { delete a_logicalDevice; }

				virtual ISwapChain* InstantiateSwapChain() = 0;
				virtual void DestroySwapChain(ISwapChain* a_swapChain) { delete a_swapChain; }

				virtual IRenderPass* InstantiateRenderPass() = 0;
				virtual void DestroyRenderPass(IRenderPass* a_renderPass) { delete a_renderPass; }
			};
		}
	}
}

#endif