#include "GamePlay/Others/Scene.h"

#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Others/GameObject.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
	namespace GamePlay
	{
		void Scene::Create(Core::Renderer* a_renderer)
		{
			m_transformSystem = new TransformSystem;

			m_meshRendererSystem = new MeshRendererSystem;

			m_resourceManager = new Resource::ResourceManager;

			ServiceLocator::ProvideResourceManager(m_resourceManager);
			ServiceLocator::ProvideTransformSystem(m_transformSystem);
			ServiceLocator::ProvideRendererSystem(m_meshRendererSystem);

			Resource::Mesh* t_mesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/viking_room.obj");
			Resource::Texture* t_texture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh->Load(a_renderer);
			t_texture->Load(a_renderer);

			GameObject* t_object = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object->AddComponent<MeshComponent>();


			t_object->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object->GetComponent<MeshComponent>()->SetTexture(t_texture);

			m_objs.push_back(t_object);
		}

		void Scene::Update(Core::Renderer* a_renderer)
		{
			m_transformSystem->Update();

			std::vector<std::pair<int, Math::mat4>> syncro;

			for (int i = 0; i < m_objs.size(); ++i)
			{
				uint32_t t_meshId = m_objs[i]->GetComponentID<MeshComponent>();
				if (t_meshId != -1)
				{
					Math::mat4 t_matrix = m_objs[i]->GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();
					syncro.push_back({ t_meshId, t_matrix });
				}
			}
			m_meshRendererSystem->Update(a_renderer, syncro);
		}

		void Scene::Draw(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Camera* a_camera)
		{
			m_meshRendererSystem->Render(a_renderer, a_window, a_camera);
		}

		void Scene::Destroy(Core::Renderer* a_renderer)
		{
			m_meshRendererSystem->Destroy(a_renderer);
			delete m_meshRendererSystem;

			delete m_transformSystem;


			m_resourceManager->DestroyAll(a_renderer);
		}
	}
}
