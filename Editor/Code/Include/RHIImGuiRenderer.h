#pragma once

//ImGui includes
#include "imgui.h"

#include <Core/Renderer/Renderer.h>
#include <Core/Window/IWindow.h>

class RHIImGuiRenderer
{
public:
	RHIImGuiRenderer() = default;
	virtual ~RHIImGuiRenderer() = default;

	virtual void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer) = 0;

	virtual void NewFrame() = 0;
	virtual void Render() = 0;
	virtual void DrawSceneAsImage() = 0;
	virtual ImDrawData* GetDrawData() = 0;
};

