#include "Core/Application.h"

#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Components/MAterialComponent.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
	namespace Core
	{
		void Application::Create(const int a_width, const int a_height)
		{
			m_mainWindow = new Window::GLwindow();
			m_mainWindow->Create(a_width, a_height);
			m_inputHandler = new Window::GLInputHandler();
			m_inputHandler->Create(m_mainWindow);

			m_renderer = new Renderer();
			m_renderer->Init(RendererType::VULKAN, m_mainWindow);

			m_currentScene = new GamePlay::Scene();
			m_currentScene->Create(m_renderer, a_width, a_height);
		}

		void Application::Run()
		{
			while (!m_mainWindow->ShouldClose())
			{
				UpdateDeltaTime();
				// Scene Update
				
				// Begin command
				
				// Begin renderPass
				// Bind pipeline
				
				// Set Render Buffer
				 
				// Draw (draw indexes)

				// End renderpass
				// End command

				// Submit

				m_currentScene->Update(m_renderer, m_mainWindow, m_inputHandler, m_deltaTime);
				m_currentScene->Draw(m_renderer, m_mainWindow);


				m_mainWindow->PollEvents();
			}
			m_renderer->WaitIdle();
		}

		void Application::Destroy()
		{
			m_currentScene->Destroy(m_renderer);
			delete m_currentScene;

			m_renderer->Destroy();
			delete m_renderer;

			m_inputHandler->Destroy();
			delete m_inputHandler;

			m_mainWindow->Destroy();
			delete m_mainWindow;
		}

		void Application::UpdateDeltaTime()
		{
			auto t_now = std::chrono::high_resolution_clock::now();
			m_deltaTime = std::chrono::duration<float>(t_now - m_lastTime).count();
			m_lastTime = t_now;
		}
	}
}
