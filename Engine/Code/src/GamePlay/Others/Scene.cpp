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

			Ref<Resource::Mesh> t_mesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/viking_room.obj");
			Ref<Resource::Texture> t_texture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh->Load(a_renderer);
			t_texture->Load(a_renderer);

			GameObject* t_object = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object->AddComponent<MeshComponent>();


			t_object->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object->GetComponent<MeshComponent>()->SetTexture(t_texture);

			AddGameObject(t_object);

			Ref<Resource::Mesh> t_mesh2 = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/FinalBaseMesh.obj");
			Ref<Resource::Texture> t_texture2 = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh2->Load(a_renderer);
			t_texture2->Load(a_renderer);

			GameObject* t_object2 = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object2->AddComponent<MeshComponent>();


			t_object2->GetComponent<MeshComponent>()->SetMesh(t_mesh2);
			t_object2->GetComponent<MeshComponent>()->SetTexture(t_texture2);

			AddGameObject(t_object2);

			GameObject* t_object3 = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object3->AddComponent<MeshComponent>();


			t_object3->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object3->GetComponent<MeshComponent>()->SetTexture(t_texture);

			AddGameObject(t_object3);

		}

		void Scene::Update(Core::Renderer* a_renderer, float a_deltaTime)
		{
			m_objs[0]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::vec3(0.05f, 0.f,0.f) * a_deltaTime);
			m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetScale(Math::vec3(0.05f));
			m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetPosition(Math::vec3(5.f, 0.f, 0.f));
			m_objs[1]->GetComponent<TransformComponent>()->SetParent(m_objs[0]->GetComponentID<TransformComponent>());
			m_objs[2]->GetComponent<TransformComponent>()->SetParent(m_objs[1]->GetComponentID<TransformComponent>());
			m_objs[2]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::vec3(0.f, 0.05f, 0.f) * a_deltaTime);
			m_transformSystem->Update();

			std::vector<std::pair<int, Math::mat4>> syncro;

			for (int i = 0; i < m_objs.size(); ++i)
			{
				uint32_t t_meshId = m_objs[i]->GetComponentID<MeshComponent>();
				if (t_meshId != -1)
				{
					Math::mat4 t_matrix = m_objs[i]->GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();
					t_matrix.Transpose(); // transpose ici
					syncro.push_back({ t_meshId, t_matrix });
				}
			}
			m_meshRendererSystem->Update(a_renderer, syncro);
			syncro.clear();
		}

		void Scene::Draw(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Camera* a_camera)
		{
			m_meshRendererSystem->Render(a_renderer, a_window, a_camera);
		}

		void Scene::Destroy(Core::Renderer* a_renderer)
		{
			m_meshRendererSystem->Destroy(a_renderer);
			delete m_meshRendererSystem;

			m_transformSystem->Destroy();
			delete m_transformSystem;


			m_resourceManager->DestroyAll(a_renderer);
			delete m_resourceManager;

			for (int i = 0; i < m_objs.size(); ++i)
			{
				delete m_objs[i];
			}
			m_objs.clear();
		}

		/**
		 * Adds game object to the scene, sets up its id and parent/child relationships.
		 * @param a_object The Object to be added to the scene
		 * @param a_parentTransformId the ids of the parent TransformComponent, if there is one
		 * @param a_childTransformIds the ids of the child TransformComponents, if there are any
		 */
		void Scene::AddGameObject(GameObject* a_object, uint32_t a_parentTransformId, std::vector<uint32_t> a_childTransformIds)
		{
			if (m_availableIds.empty())
			{
				m_objs.push_back(a_object);
				a_object->SetId(static_cast<uint32_t>(m_objs.size()) - 1);
			}
			else
			{
				m_objs[m_availableIds.back()] = a_object;
				a_object->SetId(m_availableIds.back());
				m_availableIds.pop_back();
			}

			if (a_parentTransformId != -1)
				a_object->SetParent(a_parentTransformId);
			if (!a_childTransformIds.empty())
			{
				for (uint32_t a_id : a_childTransformIds)
				{
					a_object->AddChild(a_id);
				}
			}
		}

		/**
		 * Removes specified game object from the scene.
		 * Also removes any parent/child relations the object had
		 * @param a_id the id of the game object
		 */
		void Scene::RemoveGameObject(uint32_t a_id)
		{
			for (int i = 0; i < m_objs.size(); ++i)
			{
				if (m_objs[i]->GetId() == a_id)
				{
					m_availableIds.push_back(i);
					delete m_objs[i];
					m_objs.erase(m_objs.begin() + i);
					break;
				}
			}
		}
	}
}
