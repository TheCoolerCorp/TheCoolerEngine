#ifndef VULKANINTERFACE_H
#define VULKANINTERFACE_H

#include "EngineExport.h"

#include "Core/Interfaces/ApiInterface.h"

#include "Core/Window/GLWindow.h"

namespace Engine::Core::GraphicsAPI
{
	class ENGINE_API VulkanInterface : public RHI::ApiInterface
	{
	public:
		~VulkanInterface() override = default;
		Window::IWindow* InstantiateWindow() override { return new Window::GLwindow; }
	};
}

#endif