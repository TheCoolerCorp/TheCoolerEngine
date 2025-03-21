#pragma once

//ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include <Core/Renderer/Renderer.h>
#include <Core/Window/GLWindow.h>
#include <Core/Window/IWindow.h>

using namespace Engine::Core;
using namespace Engine::Core::Window;

class RHIImGuiRenderer
{
public:
	RHIImGuiRenderer() = default;
	virtual ~RHIImGuiRenderer() = default;

	virtual void Init(IWindow* window, Renderer* renderer);

	virtual void NewFrame();
	virtual void Render();
	virtual ImDrawData* GetDrawData() = 0;
};

