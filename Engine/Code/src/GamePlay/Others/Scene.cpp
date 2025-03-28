#include "GamePlay/Others/Scene.h"

#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Others/GameObject.h"

namespace Engine
{
	namespace GamePlay
	{
		void Scene::Create(Core::Renderer& a_renderer)
		{
			m_componentsPool = new ComponentsPool;
			m_transformSystem = new TransformSystem;

			m_renderSystem = new RenderSystem;

			m_resourceManager = new Resource::ResourceManager;

			ServiceLocator::ProvideComponentsPool(m_componentsPool);
			ServiceLocator::ProvideResourceManager(m_resourceManager);

			Resource::Mesh* t_mesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/viking_room.obj");
			Resource::Texture* t_texture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh->Load(&a_renderer);
			t_texture->Load(&a_renderer);

			GameObject t_object = GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object.AddComponent<MeshComponent>();
			t_object.GetComponent<MeshComponent>()->Create();

			TransformComponent* transform_component = t_object.GetComponent<TransformComponent>();
			MeshComponent* t_component = t_object.GetComponent<MeshComponent>();

			t_object.GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object.GetComponent<MeshComponent>()->SetTexture(t_texture);
			
			m_renderSystem->Init(*m_componentsPool, a_renderer);
		}

		void Scene::Update(Core::Renderer& a_renderer)
		{
			m_transformSystem->Update(*m_componentsPool);
			m_renderSystem->Update(*m_componentsPool, a_renderer);
		}

		void Scene::Draw(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Camera* a_camera)
		{
			m_renderSystem->Render(*m_componentsPool, *a_renderer, a_window, a_camera);
		}

		void Scene::Destroy(Core::Renderer& a_renderer)
		{
			m_renderSystem->End(*m_componentsPool, a_renderer);
			delete m_renderSystem;

			delete m_transformSystem;

			m_componentsPool->Destroy();

			m_resourceManager->DestroyAll(&a_renderer);
		}
	}
}
