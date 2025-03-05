#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "EngineExport.h"

#include "Core/Window/IWindow.h"

namespace Engine::Core::RHI
{
	class ENGINE_API ApiInterface
	{
	public:
		virtual ~ApiInterface() = default;

		virtual Window::IWindow* InstantiateWindow() = 0;
		virtual void DestroyWindow(Window::IWindow* a_window) { delete a_window; }
	};
}

#endif