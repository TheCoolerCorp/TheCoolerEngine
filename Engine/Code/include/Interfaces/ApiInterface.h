#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "EngineExport.h"

#include "Window/IWindow.h"


namespace Core
{
	class ENGINE_API ApiInterface
	{
	public:
		virtual IWindow* InstantiateWindow() = 0;
		virtual void DestroyWindow(IWindow* a_window) { delete a_window; }
	};
}
#endif