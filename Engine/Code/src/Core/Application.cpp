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

			Resource::ResourceManager& instance = Resource::ResourceManager::Get();
			instance.CreateResource(Resource::ResourceType::MESH, "Assets/Meshes/viking_room.obj", "viking_room_mesh");
			instance.CreateResource(Resource::ResourceType::TEXTURE, "Assets/Textures/viking_room.png", "viking_room_texture");
			instance.DestroyResource("viking_room_mesh");
			instance.DestroyResource("viking_room_texture");
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
