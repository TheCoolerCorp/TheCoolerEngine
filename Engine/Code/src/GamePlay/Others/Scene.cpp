#include <utility>

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

			m_physicsSystem = new PhysicsSystem;
			m_physicsSystem->Create();

			m_resourceManager = new Resource::ResourceManager;

			ServiceLocator::ProvideResourceManager(m_resourceManager);
			ServiceLocator::ProvideTransformSystem(m_transformSystem);
			ServiceLocator::ProvideRendererSystem(m_meshRendererSystem);
			ServiceLocator::ProvidePhysicsSystem(m_physicsSystem);

			m_mainCamera = new Camera(Math::vec3(0.f, 1.f, 0.f), Math::vec3(0.f, 0.f, 0.f),
				Math::vec3(0.f, 1.f, 3.f), Math::ToRadians(70.f),
				static_cast<float>(a_width) / static_cast<float>(a_height), 0.1f, 100.f, 10.f, 2.f);
			m_mainCamera->Create(a_renderer);

			Ref<Resource::Mesh> t_mesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/viking_room.obj");
			Ref<Resource::Texture> t_texture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh->Load(a_renderer);
			t_texture->Load(a_renderer);

			GameObject* t_object = new GameObject(Math::vec3(0.f, 0.f, 0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object->AddComponent<MeshComponent>();
			t_object->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object->GetComponent<MeshComponent>()->GetMaterial()->SetType(UNLIT);
			t_object->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo(t_texture);

			GameObject* t_object2 = new GameObject(Math::vec3(5.f, 0.f, 0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object2->AddComponent<MeshComponent>();
			t_object2->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object2->GetComponent<MeshComponent>()->GetMaterial()->SetType(LIT);
			t_object2->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo(t_texture);

			AddGameObject(t_object);
			AddGameObject(t_object2);

		}

		void Scene::Update(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, float a_deltatime)
		{
			m_objs[0]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::quat(Math::vec3(0.01f * a_deltatime, 0.f,0.f)));
			//m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetScale(Math::vec3(0.05f));
			//m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetPosition(Math::vec3(5.f, 0.f, 0.f));
			//m_objs[1]->GetComponent<TransformComponent>()->SetParent(m_objs[0]->GetComponentID<TransformComponent>());
			//m_objs[2]->GetComponent<TransformComponent>()->SetParent(m_objs[1]->GetComponentID<TransformComponent>());
			//m_objs[2]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::vec3(0.f, 0.05f, 0.f));
			m_transformSystem->Update();

			std::vector<std::pair<int, Math::mat4>> t_syncro;
			std::vector<Math::Transform*> t_physicsTransforms;
			m_mainCamera->Update(a_renderer, a_inputHandler, a_window, a_deltatime);

			for (GameObject* t_obj : m_objs)
			{
				const uint32_t t_rigidBodyId = t_obj->GetComponentID<RigidBodyComponent>();
				if (std::cmp_not_equal(t_rigidBodyId, -1))
				{
					Math::Transform* t_transform = t_obj->GetComponent<TransformComponent>()->GetTransform();
					t_physicsTransforms.push_back(t_transform);
				}
				t_obj->UpdateColliderMat();
			}
			m_physicsSystem->Update(a_deltatime, t_physicsTransforms);

			for (GameObject* t_obj : m_objs)
			{
				const int t_meshId = t_obj->GetComponentID<MeshComponent>();
				if (std::cmp_not_equal(t_meshId, -1))
				{
					Math::mat4 t_matrix = t_obj->GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();
					t_matrix.Transpose();
					t_syncro.emplace_back(t_meshId, t_matrix);
				}
				const int t_colliderMeshId = t_obj->GetComponentID<MeshComponent>(true);
				if (std::cmp_not_equal(t_colliderMeshId, -1))
				{
					Math::mat4 t_matrix;
					if (t_obj->GetComponent<RigidBodyComponent>()->GetDebug())
					{
						t_matrix = t_obj->GetColliderMat();
						t_matrix.Transpose();
					}
					else
					{
						t_matrix = Math::mat4(false);
					}
					t_syncro.emplace_back(t_colliderMeshId, t_matrix);
				}
			}
			m_meshRendererSystem->Update(a_renderer, t_syncro);

			t_physicsTransforms.clear();
			t_syncro.clear();
		}

		/**
		 * Returns all vertex buffers in the scene, grouped by their pipeline target type.
		 * @return std::unordered_map with the format <PipelineTarget, std::vector<Core::RHI::IBuffer*>> for fast acess
		 */
		std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> Scene::GetVertexBuffers()
		{
			std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> t_vertexBuffersMap;
			for (int i = 0; i < m_meshRendererSystem->GetComponents().size(); ++i)
			{
				t_vertexBuffersMap[GetDescriptorTarget(i)].push_back(m_meshRendererSystem->GetComponents().at(i)->GetMesh()->GetVertexBuffer());
			}
			return t_vertexBuffersMap;
		}

		/**
		 * Returns all index buffers in the scene, grouped by their pipeline target type.
		 * @return std::unordered_map with the format <PipelineTarget, std::vector<Core::RHI::IBuffer*>> for fast acess
		 */
		std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> Scene::GetIndexBuffers()
		{
			std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> t_indexBuffersMap;
			for (int i = 0; i < m_meshRendererSystem->GetComponents().size(); ++i)
			{
				t_indexBuffersMap[GetDescriptorTarget(i)].push_back(m_meshRendererSystem->GetComponents().at(i)->GetMesh()->GetIndexBuffer());
			}
			return t_indexBuffersMap;
		}

		/**
		 * Returns all indices in the scene, grouped by their pipeline target type.
		 * @return std::unordered_map with the format <PipelineTarget, std::vector<uint32_t>> for fast acess
		 */
		std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>
		Scene::GetNBIndices()
		{
			std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>> t_nbIndicesMap;
			for (int i = 0; i < m_meshRendererSystem->GetComponents().size(); ++i)
			{
				t_nbIndicesMap[GetDescriptorTarget(i)].push_back(m_meshRendererSystem->GetComponents().at(i)->GetMesh()->GetNbIndices());
			}
			return t_nbIndicesMap;
		}

		/**
		 * Returns all descriptors in the scene, grouped by their pipeline target type.
		 * @return std::unordered_map with the format <PipelineTarget, std::vector<Core::RHI::IObjectDescriptor*>> for fast acess
		 */
		std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IObjectDescriptor*>>
		Scene::GetDescriptors()
		{
			std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IObjectDescriptor*>> t_descriptorsMap;
			std::vector<Core::RHI::IObjectDescriptor*>& t_descriptors = m_meshRendererSystem->GetDescriptors();
			for(auto & t_descriptor : t_descriptors)
			{
				t_descriptorsMap[t_descriptor->GetPipelineTargetType()].push_back(t_descriptor);
			}
			return t_descriptorsMap;
		}

		/**
		 * Returns the decriptor's pipeline target type at the specified index.
		 * @param a_idx the index of the descriptor
		 * @return the descriptor's pipeline target type
		 */
		Core::RHI::DescriptorSetPipelineTarget Scene::GetDescriptorTarget(int a_idx)
		{
			if ( Core::RHI::IObjectDescriptor* t_descriptor = m_meshRendererSystem->GetDescriptor(a_idx))
			{
				return t_descriptor->GetPipelineTargetType();
			}
			LOG_ERROR("Tired to get descriptor at an invalid index!");
			return Core::RHI::DescriptorSetPipelineTarget::LitDescriptor;
		}


		void Scene::Destroy(Core::Renderer* a_renderer)
		{
			m_meshRendererSystem->Destroy(a_renderer);
			delete m_meshRendererSystem;

			m_transformSystem->Destroy();
			delete m_transformSystem;

			m_physicsSystem->Destroy();
			delete m_physicsSystem;
			m_physicsSystem = nullptr;

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
