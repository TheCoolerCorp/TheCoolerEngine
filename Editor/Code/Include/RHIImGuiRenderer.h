#pragma once

#include <Core/Renderer/Renderer.h>
#include <Core/Window/GLWindow.h>
#include <Core/Window/IWindow.h>

using namespace Engine::Core;
using namespace Engine::Core::Window;

class RHIImGuiRenderer
{
public:
	RHIImGuiRenderer() = default;
	~RHIImGuiRenderer() = default;

	virtual void Init(IWindow* window, Renderer* renderer);

	virtual void NewFrame();
	virtual void Render();
};

