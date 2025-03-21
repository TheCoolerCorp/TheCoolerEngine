#include "Core/Application.h"

#include "GamePlay/GameObject.h"
#include "GamePlay/TextureComponent.h"
#include "GamePlay/Meshcomponent.h"
#include "Math/TheCoolerMath.h"

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
			GamePlay::GameObject* t_obj = new GamePlay::GameObject(Math::vec3(0.f, 0.f, 0.f), Math::quat(Math::vec3(Math::ToRadians(90.f), Math::ToRadians(90.f), 0.f)), Math::vec3(0.5f, 0.5f, 0.5f));
			t_obj->AddComponent<GamePlay::MeshComponent>("Assets/Meshes/viking_room.obj", { m_renderer->GetLogicalDevice(), m_renderer->GetPhysicalDevice(), m_renderer->GetPipeline(), m_renderer->GetDescriptorPool(), m_renderer->GetCommandPool(), 3 }, m_renderer->GetInterface());
			t_obj->AddComponent<GamePlay::TextureComponent>("Assets/Textures/viking_room.png", { m_renderer->GetLogicalDevice(), m_renderer->GetPhysicalDevice(), m_renderer->GetPipeline(), m_renderer->GetDescriptorPool(), m_renderer->GetCommandPool(), 3 }, m_renderer->GetInterface());
			t_obj->Create(m_renderer->GetInterface(), { m_renderer->GetLogicalDevice(), m_renderer->GetPhysicalDevice(), m_renderer->GetPipeline(), m_renderer->GetDescriptorPool(), m_renderer->GetCommandPool(), 3 });
			m_gameObjectDatas.push_back(t_obj->SubmitData());
			m_gameObjects.push_back(t_obj);
		}

		void Application::Run()
		{
			while (!m_mainWindow->ShouldClose())
			{
				m_gameObjectDatas.clear();
				for (GamePlay::GameObject* t_gameObject : m_gameObjects)
				{
					t_gameObject->Update(m_renderer->GetSwapChain()->GetCurrentFrame(), m_renderer->GetLogicalDevice());
					m_gameObjectDatas.push_back(t_gameObject->SubmitData());
				}
				m_mainWindow->PollEvents();
				m_renderer->Run(m_mainWindow, m_gameObjectDatas);
			}
			m_renderer->WaitIdle();
		}

		void Application::Destroy()
		{
			m_gameObjectDatas.clear();

			for (int i = 0; i < m_gameObjects.size(); ++i)
			{
				m_gameObjects[i]->Destroy(m_renderer->GetInterface(), m_renderer->GetLogicalDevice());
				delete m_gameObjects[i];
			}
			m_gameObjects.clear();

			m_renderer->Destroy();
			delete m_renderer;

			m_mainWindow->Destroy();
			delete m_mainWindow;
		}
	}
}
