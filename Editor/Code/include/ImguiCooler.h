#ifndef IMGUICOOLER_H
#define  IMGUICOOLER_H
#include "Core/Window/IWindow.h"
#include  "Core/Renderer/Renderer.h"

class UI
{
public:
	static void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer);
	static void Update(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer);
};

#endif