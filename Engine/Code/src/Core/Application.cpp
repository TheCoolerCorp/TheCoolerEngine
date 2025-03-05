#include "Core/Application.h"

namespace Core
{
	void Application::Create(int a_width, int a_height)
	{
		m_apiInterface = new VulkanInterface();
		m_window = m_apiInterface->InstantiateWindow();
		m_window->Create(a_width, a_height);
	}

	void Application::Run()
	{
		while (!m_window->ShouldClose())
		{
			m_window->PollEvents();
		}
	}

	void Application::Destroy()
	{
		m_window->Destroy();
		m_apiInterface->DestroyWindow(m_window);
		
		delete m_apiInterface;
	}
}
