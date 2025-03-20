#include "Core/Application.h"

#include "GamePlay/GameObject.h"
#include "GamePlay/TextureComponent.h"
#include "GamePlay/Meshcomponent.h"
namespace Engine
{
	namespace Core
	{
		void Application::Create(const int a_width, const int a_height)
		{
			m_mainWindow = new Window::GLwindow();
			m_mainWindow->Create(a_width, a_height);

			m_renderer = new Renderer();
			m_renderer->Init(RendererType::VULKAN, m_mainWindow);
			GamePlay::GameObject t_obj = GamePlay::GameObject(Math::vec3(0.f), Math::quat(0.f, 0.f, 0.f, 1.f), Math::vec3(0.5f, 0.5f, 0.5f));
			t_obj.AddComponent<GamePlay::MeshComponent>("Assets/Meshes/viking_room.obj");
			t_obj.AddComponent<GamePlay::TextureComponent>("Assets/Textures/viking_room.png");
			t_obj.Create(m_renderer->GetInterface(), { m_renderer->GetLogicalDevice(), m_renderer->GetPhysicalDevice(), m_renderer->GetPipeline(), m_renderer->GetDescriptorPool(), m_renderer->GetCommandPool(), 3 });
			m_gameObjectDatas.push_back(t_obj.SubmitData());
		}

		void Application::Run() const
		{
			while (!m_mainWindow->ShouldClose())
			{
				m_mainWindow->PollEvents();
				m_renderer->Run(m_mainWindow, m_gameObjectDatas);
			}
			m_renderer->WaitIdle();
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
