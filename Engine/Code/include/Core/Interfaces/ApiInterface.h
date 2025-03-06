#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "EngineExport.h"

#include "Core/Window/IWindow.h"
#include "Core/Interfaces/IInstance.h"
#include "Core/Interfaces/IValidationLayers.h"

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
				virtual void DestroyInstance(RHI::IInstance* a_instance) { delete a_instance; }

				
				virtual IValidationLayers* InstantiateValidationLayers() = 0;
				virtual void DestroyValidationLayers(RHI::IValidationLayers* a_validationLayers) { delete a_validationLayers; }
			};
		}
	}
}

#endif