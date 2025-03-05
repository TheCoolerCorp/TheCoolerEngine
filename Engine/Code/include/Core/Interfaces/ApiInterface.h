#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "EngineExport.h"

#include "Core/Window/IWindow.h"


namespace Core
{
	class ENGINE_API ApiInterface
	{
	public:
		virtual ~ApiInterface() = default;

		virtual IWindow* InstantiateWindow() = 0;
		virtual void DestroyWindow(IWindow* a_window) { delete a_window; }
	};
}
#endif