#include "Core/Application.h"
#include "Ressources/ResourceManager.h"
namespace Engine
{
	namespace Core
	{
		void Application::Create(int a_width, int a_height)
		{
			m_mainWindow = new Window::GLwindow();
			m_mainWindow->Create(a_width, a_height);

			m_renderer = new Renderer();
			m_renderer->Init(RendererType::VULKAN, m_mainWindow);
		}

		void Application::Run() const
		{
			while (!m_mainWindow->ShouldClose())
			{
				m_mainWindow->PollEvents();
				m_renderer->Run();
			}
		}

		void Application::Destroy() const
		{
			m_renderer->Destroy();
			delete m_renderer;

			m_mainWindow->Destroy();
			delete m_mainWindow;
		}
	}
}
