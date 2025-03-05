#ifndef VULKANINTERFACE_H
#define VULKANINTERFACE_H

#include "EngineExport.h"

#include "ApiInterface.h"

#include "Core/Window/GLWindow.h"

namespace Core
{
	class ENGINE_API VulkanInterface : public ApiInterface
	{
	public:
		~VulkanInterface() override = default;
		IWindow* InstantiateWindow() override { return new GLwindow; }
	};
}
#endif