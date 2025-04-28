#include "GamePlay/Others/Scene.h"

#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Others/GameObject.h"
#include "Math/TheCoolerMath.h"
#include "Core/GraphicsAPI/Vulkan/VulkanShader.h"
namespace Engine
{
	namespace GamePlay
	{
		void Scene::Create(Core::Renderer* a_renderer, int a_width, int a_height)
		{
			m_transformSystem = new TransformSystem;

			m_meshRendererSystem = new MeshRendererSystem;

			m_resourceManager = new Resource::ResourceManager;

			ServiceLocator::ProvideResourceManager(m_resourceManager);
			ServiceLocator::ProvideTransformSystem(m_transformSystem);
			ServiceLocator::ProvideRendererSystem(m_meshRendererSystem);

			m_mainCamera = new Camera(Math::vec3(0.f, 1.f, 0.f), Math::vec3(0.f, 0.f, 0.f),
				Math::vec3(0.f, 1.f, 3.f), Math::ToRadians(70.f),
				static_cast<float>(a_width) / static_cast<float>(a_height), 0.1f, 100.f, 10.f, 2.f);
			m_mainCamera->Create(a_renderer);
			
			#pragma region ObjectTest
			Ref<Resource::Mesh> t_mesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/viking_room.obj");
			Ref<Resource::Texture> t_texture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh->Load(a_renderer);
			t_texture->Load(a_renderer);

			GameObject* t_object = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object->AddComponent<MeshComponent>();
			t_object->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object->GetComponent<MeshComponent>()->GetMaterial()->SetType(UNLIT);
			t_object->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo(t_texture);

			GameObject* t_object2 = new GameObject(Math::vec3(5.f,0.f,0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object2->AddComponent<MeshComponent>();
			t_object2->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object2->GetComponent<MeshComponent>()->GetMaterial()->SetType(LIT);
			t_object2->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo(t_texture);

			m_objs.push_back(t_object);
			m_objs.push_back(t_object2);

			/*Ref<Resource::Mesh> t_mesh2 = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/FinalBaseMesh.obj");
			t_mesh2->Load(a_renderer);

			GameObject* t_object2 = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object2->AddComponent<MeshComponent>();


			t_object2->GetComponent<MeshComponent>()->SetMesh(t_mesh2);
			t_object2->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo(t_texture);

			m_objs.push_back(t_object2);

			GameObject* t_object3 = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object3->AddComponent<MeshComponent>();


			t_object3->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object3->GetComponent<MeshComponent>()->GetMaterial()->SetType(LIT);
			t_object3->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo(t_texture);

			m_objs.push_back(t_object3);*/
			#pragma endregion
		}

		void Scene::Update(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, float a_deltatime)
		{
			m_objs[0]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::vec3(0.05f, 0.f,0.f));
			//m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetScale(Math::vec3(0.05f));
			//m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetPosition(Math::vec3(5.f, 0.f, 0.f));
			//m_objs[1]->GetComponent<TransformComponent>()->SetParent(m_objs[0]->GetComponentID<TransformComponent>());
			//m_objs[2]->GetComponent<TransformComponent>()->SetParent(m_objs[1]->GetComponentID<TransformComponent>());
			//m_objs[2]->GetComponent<TransformComponent>()->GetTransform()->SetPosition(Math::vec3(5.f, 0.f, 0.f));
			//m_objs[2]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::vec3(0.f, 0.05f, 0.f));

			m_transformSystem->Update();

			m_mainCamera->Update(a_renderer, a_inputHandler, a_window, a_deltatime);
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

		std::vector<Core::RHI::IBuffer*> Scene::GetVertexBuffers()
		{
			std::vector<Core::RHI::IBuffer*> t_vertexBuffers;

			for (int i = 0; i < m_meshRendererSystem->GetComponents().size(); ++i)
			{
				t_vertexBuffers.push_back(m_meshRendererSystem->GetComponents().at(i)->GetMesh()->GetVertexBuffer());
			}
			return t_vertexBuffers;
		}

		std::vector<Core::RHI::IBuffer*> Scene::GetIndexBuffers()
		{
			std::vector<Core::RHI::IBuffer*> t_indexBuffers;

			for (int i = 0; i < m_meshRendererSystem->GetComponents().size(); ++i)
			{
				t_indexBuffers.push_back(m_meshRendererSystem->GetComponents().at(i)->GetMesh()->GetIndexBuffer());

			}
			return t_indexBuffers;
		}

		std::vector<uint32_t> Scene::GetNBIndices()
		{
			std::vector<uint32_t> t_nbIndices;

			for (int i = 0; i < m_meshRendererSystem->GetComponents().size(); ++i)
			{
				t_nbIndices.push_back(m_meshRendererSystem->GetComponents().at(i)->GetMesh()->GetNbIndices());

			}
			return t_nbIndices;
		}

		std::vector<Core::RHI::IObjectDescriptor*> Scene::GetDescriptors()
		{
			return m_meshRendererSystem->GetDescriptors();
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

			m_mainCamera->Destroy(a_renderer);
			delete m_mainCamera;
		}
	}
}
