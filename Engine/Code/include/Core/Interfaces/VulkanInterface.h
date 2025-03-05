#ifndef VULKANINTERFACE_H
#define VULKANINTERFACE_H

#include "EngineExport.h"

#include "Interfaces/ApiInterface.h"

#include "Window/GLWindow.h"

namespace Core
{
	class ENGINE_API VulkanInterface : public ApiInterface
	{
	public:
		IWindow* InstantiateWindow() override { return new GLwindow; }
	};
}
#endif