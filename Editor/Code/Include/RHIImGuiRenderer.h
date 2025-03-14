#pragma once

#include <Core/Renderer/Renderer.h>
#include <Core/Window/GLWindow.h>

using namespace Engine::Core;
using namespace Engine::Core::Window;
/*
virtual class for any 3d renderer-specific functions to comply with the RHI
*/
class RHIImGuiRenderer
{
public:
	virtual void Init(IWindow* window, Renderer* renderer) = 0;

	virtual void NewFrame() = 0;
	virtual void Render() = 0;
};

