#include "GamePlay/Others/Scene.h"

#include <utility>
#include <meta/factory.hpp>
#include <meta/meta.hpp>

#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Components/PlayerControllerComponent.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Others/CameraGO.h"
#include "Math/TheCoolerMath.h"
#include "GamePlay/Components/ReflectionComponents/ComponentRegistry.h"

using json = nlohmann::ordered_json;

namespace Engine
{
	namespace GamePlay
	{
		bool Scene::m_processKeyboardInputs = true;

		void Scene::Create(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, const char* a_name, int a_width, int a_height)
		{
			m_renderer = a_renderer;
			m_name = a_name;

			m_width = a_width;
			m_height= a_height;
			m_transformSystem = new TransformSystem;

			m_renderSystem = new RenderSystem;
			m_renderSystem->Create(a_renderer);

			m_physicsSystem = new PhysicsSystem;
			m_physicsSystem->Create();

			m_gameComponentSystem = new GameComponentSystem;
			m_gameComponentSystem->Create(this, a_window);

			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();
			ServiceLocator::ProvideTransformSystem(m_transformSystem);
			ServiceLocator::ProvideRenderSystem(m_renderSystem);
			ServiceLocator::ProvidePhysicsSystem(m_physicsSystem);
			ServiceLocator::ProvideGameComponentSystem(m_gameComponentSystem);

			Ref<Resource::Texture> t_defaultTexture= t_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
			t_defaultTexture->Load();
			t_defaultTexture->CreateImage(a_renderer);

			m_mainCamera = new Camera(Math::vec3(0.f, 1.f, 0.f), Math::vec3(0.f, 0.f, 0.f),
				Math::vec3(0.f, 0.f, 3.f), Math::ToRadians(70.f),
				static_cast<float>(a_width) / static_cast<float>(a_height), 0.1f, 100.f, 10.f, 20.f);
			m_mainCamera->Create(a_renderer);

			Load(a_renderer);
		}

		void Scene::Update(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, float a_deltatime)
		{
			while (!m_deletionQueue.empty())
			{
				int t_id = m_deletionQueue.back();
				m_deletionQueue.pop_back();
				for (int i = 0; i < m_objs.size(); ++i)
				{
					if (!m_objs[i])
						continue;
					if (m_objs[i]->GetId() == t_id)
					{
						m_availableIds.push_back(i);
						delete m_objs[i];
						m_objs[i] = nullptr;
						break;
					}
				}
			}


			m_transformSystem->Update();

			std::vector<std::pair<int, Math::UniformMatrixs>> t_syncro;
			std::vector < std::tuple<int, Math::vec3, Math::vec3 >> t_lightSyncro; // index, pos, rot
			std::vector<std::pair<int, Math::mat4>> t_cameraSyncro;
			std::vector<int> t_materialUpdate;

			for (GameObject* t_obj : m_objs)
			{
				if (!t_obj)
				{
					t_syncro.emplace_back(-1, Math::UniformMatrixs(Math::mat4(true), Math::mat4(true)));
					continue;
				}
				t_obj->UpdateColliderMat();

				const int t_meshId = t_obj->GetComponentID<MeshComponent>();
				if (std::cmp_not_equal(t_meshId, -1))
				{
					Ref<Resource::Mesh> t_mesh = t_obj->GetComponent<MeshComponent>()->GetMesh();
					if (!t_obj->GetComponent<MeshComponent>()->GetMesh()->IsBound())
					{
						t_mesh->BindBuffers(a_renderer);
					}
					t_syncro.emplace_back(t_meshId, t_obj->GetComponent<TransformComponent>()->GetTransform()->GetUniformsMatrixs());
					if (m_renderSystem->GetMeshComponent(t_meshId)->GetMaterial()->GetNeedUpdate())
					{
						t_materialUpdate.push_back(t_meshId);
					}
				}

				if (const RigidBodyComponent* t_rigidBodyComponent = t_obj->GetComponent<RigidBodyComponent>(); t_rigidBodyComponent)
				{
					if (const int t_colliderMeshId = t_obj->GetComponent<RigidBodyComponent>()->GetMeshID(); t_colliderMeshId != -1)
					{
						Math::UniformMatrixs t_uniformMatrixs = {
							.m_transform= t_rigidBodyComponent->GetDebug() ? t_obj->GetColliderMat() : Math::mat4(),
							.m_normalMatrix= Math::mat4()
						};
						t_syncro.emplace_back(t_colliderMeshId, t_uniformMatrixs);
					}
				}

				const int t_lightId = t_obj->GetComponentID<LightComponent>();
				if (std::cmp_not_equal(t_lightId, -1))
				{
					t_lightSyncro.emplace_back( t_lightId, t_obj->GetComponent<TransformComponent>()->GetTransform()->GetGlobalPosition(),Math::quat::ToEulerAngles(t_obj->GetComponent<TransformComponent>()->GetTransform()->GetGlobalRotation()) );	
				}
			}

			m_physicsSystem->UpdatesFromTransforms(this);

			if (!m_isPlaying)
			{
				if (m_lastState != m_isPlaying)
				{
					m_mainCamera->SetFreeCam(true);
					Reload(a_renderer);
				}
				m_mainCamera->Update(a_renderer, a_inputHandler, a_window, a_deltatime);
				m_gameComponentSystem->SceneUpdate();
			}
			else
			{
				/*
				*	DO SOME IN GAME UPDATE STUFF
				*
				*/
				if (m_lastState != m_isPlaying)
				{
					m_mainCamera->SetFreeCam(false);
					Save();
					m_lastState = m_isPlaying;
					m_physicsSystem->SetComponentsForPlay();
				}
				m_physicsSystem->Update(a_deltatime, this);
				m_renderSystem->UpdateCamera(a_renderer, a_deltatime, a_window, a_inputHandler, m_gameCameraId, m_objs[m_mainCameraObjectId] ? m_objs[m_mainCameraObjectId]->GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix() : Math::mat4());
				m_gameComponentSystem->Update(a_deltatime);
				if (m_processKeyboardInputs)
				{
					m_gameComponentSystem->ProcessInputs(a_inputHandler, a_deltatime);
				}
			}


			if (!m_justReloaded)
			{
				m_renderSystem->Update(a_renderer, t_syncro, t_lightSyncro, t_materialUpdate, t_cameraSyncro);
			}

			t_syncro.clear();
			t_lightSyncro.clear();
			t_materialUpdate.clear();
			t_cameraSyncro.clear();
			m_justReloaded = false;
		}

		/**
		 * Returns all vertex buffers in the scene, grouped by their pipeline target type.
		 * @return std::unordered_map with the format <PipelineTarget, std::vector<Core::RHI::IBuffer*>> for fast acess
		 */
		std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> Scene::GetVertexBuffers()
		{
			std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> t_vertexBuffersMap;
			for (int i = 0; i < m_renderSystem->GetMeshComponents().size(); ++i)
			{
				MeshComponent* t_meshComponent = m_renderSystem->GetMeshComponents().at(i);
				if (!t_meshComponent || !t_meshComponent->GetMesh()->IsLoaded())
				{
					continue;
				}
				t_vertexBuffersMap[GetDescriptorTarget(i)].push_back(t_meshComponent->GetMesh()->GetVertexBuffer());
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
			for (int i = 0; i < m_renderSystem->GetMeshComponents().size(); ++i)
			{
				MeshComponent* t_meshComponent = m_renderSystem->GetMeshComponents().at(i);
				if (!t_meshComponent || !t_meshComponent->GetMesh()->IsLoaded())
				{
					continue;
				}
				t_indexBuffersMap[GetDescriptorTarget(i)].push_back(t_meshComponent->GetMesh()->GetIndexBuffer());
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
			for (int i = 0; i < m_renderSystem->GetMeshComponents().size(); ++i)
			{
				MeshComponent* t_meshComponent = m_renderSystem->GetMeshComponents().at(i);
				if (!t_meshComponent || !t_meshComponent->GetMesh()->IsLoaded())
				{
					continue;
				}
				t_nbIndicesMap[GetDescriptorTarget(i)].push_back(t_meshComponent->GetMesh()->GetNbIndices());
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
			std::vector<Core::RHI::IObjectDescriptor*>& t_descriptors = m_renderSystem->GetMeshDescriptors();
			for(Core::RHI::IObjectDescriptor* t_descriptor : t_descriptors)
			{
				if (t_descriptor == nullptr)
					continue;
				t_descriptorsMap[t_descriptor->GetPipelineTargetType()].push_back(t_descriptor);
			}
			return t_descriptorsMap;
		}

		Core::RHI::IObjectDescriptor* Scene::GetLightsDescriptors()
		{
			return m_renderSystem->GetLightDescriptor();
		}

		/**
		 * Returns the decriptor's pipeline target type at the specified index.
		 * @param a_idx the index of the descriptor
		 * @return the descriptor's pipeline target type
		 */
		Core::RHI::DescriptorSetPipelineTarget Scene::GetDescriptorTarget(int a_idx)
		{
			if ( Core::RHI::IObjectDescriptor* t_descriptor = m_renderSystem->GetMeshDescriptor(a_idx))
			{
				return t_descriptor->GetPipelineTargetType();
			}
			return Core::RHI::DescriptorSetPipelineTarget::LitDescriptor;
		}


		void Scene::Destroy(Core::Renderer* a_renderer)
		{
			for (int i = 0; i < m_objs.size(); ++i)
			{
				delete m_objs[i];
			}
			m_objs.clear();

			m_renderSystem->Destroy(a_renderer);
			delete m_renderSystem;

			m_transformSystem->Destroy();
			delete m_transformSystem;

			m_physicsSystem->Destroy();
			delete m_physicsSystem;
			m_physicsSystem = nullptr;

			m_mainCamera->Destroy(a_renderer);
			delete m_mainCamera;
		}

		Core::RHI::IObjectDescriptor* Scene::GetCameraDescriptor()
		{
			if (IsPlaying())
			{
				if (m_renderSystem->GetCameraDescriptor(m_gameCameraId))
				{
					return m_renderSystem->GetCameraDescriptor(m_gameCameraId);
				}
			}

			if (m_mainCamera->GetDescriptor())
			{
				return m_mainCamera->GetDescriptor();
			}
			return nullptr;
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
				if (m_objs.empty())
				{
					m_objs.push_back(a_object);
					a_object->SetId(static_cast<uint32_t>(m_objs.size()) - 1);
					m_availableIds.clear();
				}
				else
				{
					m_objs[m_availableIds.back()] = a_object;
					a_object->SetId(m_availableIds.back());
					m_availableIds.pop_back();
				}
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

		GameObject* Scene::AddGameObject(GameObjectType a_type, uint32_t a_parentTransformId,
			std::vector<uint32_t> a_childTransformIds)
		{
			GameObject* t_object = nullptr;
			t_object = new GameObject(Math::vec3(0.f, 0.f, 0.f), Math::vec3(0.f, 0.f, 0.f), Math::vec3(1.f));
			AddGameObject(t_object, a_parentTransformId, a_childTransformIds);
			switch (a_type)
			{
			case OBJECTTYPE_EMPTY:
				t_object->SetName("Empty");
				break;
			case OBJECTTYPE_CUBE:
				t_object->SetName("Cube");
				t_object->AddComponent<MeshComponent>();
				t_object->GetComponent<MeshComponent>()->SetMesh("Assets/Meshes/BaseObjects/Cube.obj", m_renderer);
				break;
			case OBJECTTYPE_SPHERE:
				t_object->SetName("Sphere");
				t_object->AddComponent<MeshComponent>();
				t_object->GetComponent<MeshComponent>()->SetMesh("Assets/Meshes/BaseObjects/Sphere.obj", m_renderer);
				break;
			case OBJECTTYPE_PLANE:
				t_object->SetName("Plane");
				t_object->AddComponent<MeshComponent>();
				t_object->GetComponent<MeshComponent>()->SetMesh("Assets/Meshes/BaseObjects/plane.obj", m_renderer);
				break;
			case OBJECTTYPE_LIGHT:
				t_object->SetName("Light");
				t_object->AddComponent<LightComponent>();
				break;
			case OBJECTTYPE_CAMERA:
				t_object->SetName("Camera");
				t_object->AddComponent<CameraComponent>();
				t_object->GetComponent<CameraComponent>()->GetCamera().Set(Math::vec3(0.f, 1.f, 0.f), Math::vec3(0.f, 0.f, 0.f),
						Math::vec3(0.f, 1.f, 3.f), Math::ToRadians(70.f),
						static_cast<float>(m_width) / static_cast<float>(m_height), 0.1f, 100.f, 10.f, 20.f);
				t_object->GetComponent<CameraComponent>()->GetCamera().Create(m_renderer);
				SetMainCamera(t_object->GetId());
				break;
			default:
				break;
			}
			if (t_object && t_object->GetComponent<MeshComponent>())
			{
				t_object->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo("Assets/Textures/BaseObjectTexture.png", m_renderer);
				t_object->GetComponent<MeshComponent>()->GetMaterial()->SetType(UNLIT);
			}
			return t_object;
		}

		/**
		 * Removes specified game object from the scene.
		 * Also removes any parent/child relations the object had
		 * @param a_id the id of the game object
		 */
		void Scene::RemoveGameObject(uint32_t a_id)
		{
			m_deletionQueue.push_back(a_id);
			
		}

		void Scene::Save()
		{
			json t_scene;

			for (const auto& t_obj : m_objs)
			{
				if (!t_obj)
					continue;
				json t_objJson;

				int newIndex = 0;
				std::vector<std::pair<int, int>> t_oldAndNewIndexes;




				for (int oldIndex = 0; oldIndex < m_transformSystem->GetSize(); ++oldIndex)
				{
					if (m_transformSystem->GetComponent(oldIndex) != nullptr)
					{
						if (oldIndex != newIndex)
						{
							t_oldAndNewIndexes.emplace_back(oldIndex, newIndex);
						}
						++newIndex;
					}
				}


				t_objJson["GameObject"] = t_obj->GetName();
				t_objJson["TransformComponent"] = SerializeTransformComponent(*t_obj->GetComponent<TransformComponent>(), t_oldAndNewIndexes);


				if (const RigidBodyComponent* t_rigidBodyComponent = t_obj->GetComponent<RigidBodyComponent>())
				{
					t_objJson["RigidBodyComponent"] = SerializeRigidBodyComponent(*t_rigidBodyComponent);
				}
				if (const LightComponent* t_lightComponent = t_obj->GetComponent<LightComponent>())
				{
					t_objJson["LightComponent"] = SerializeLightComponent(*t_lightComponent);
				}
				if (const MeshComponent* t_meshComponent = t_obj->GetComponent<MeshComponent>())
				{
					t_objJson["MeshComponent"] = SerializeMeshComponent(*t_meshComponent);
				}
				if (const CameraComponent* t_cameraComponent = t_obj->GetComponent<CameraComponent>())
				{
					t_objJson["CameraComponent"] = SerializeCameraComponent(*t_cameraComponent);
				}
				if (const PlayerControllerComponent* t_playerControllerComponent = t_obj->GetComponent<PlayerControllerComponent>())
				{
					t_objJson["PlayerControllerComponent"] = SerializePlayerControllerComponent(*t_playerControllerComponent);
				}

				for (auto& t_typeIndex : t_obj->GetOwnedTypes())
				{
					ComponentRegistry::Entry* t_entry = ComponentRegistry::Instance().GetEntryFromId(t_typeIndex);
					if (t_entry)
					{
						GameComponent* t_comp = t_entry->getComponent(*t_obj);
						t_objJson[t_entry->name] = t_comp->Serialize();
					}
				}

				t_scene.push_back(t_objJson);
			}

			std::ofstream t_file("Assets/Scenes/" + m_name + ".json");
			if (t_file.is_open())
			{
				t_file << t_scene.dump(4);
				t_file.close();
			}
		}

		void Scene::Load(Core::Renderer* a_renderer)
		{
			//std::ifstream t_file("Assets/Scenes/" + m_name + ".json");
			std::ifstream t_file("Assets/Scenes/" + m_name + ".json");
			if (!t_file.is_open())
			{
				return;
			}

			json t_scene;
			t_file >> t_scene;

			std::vector<TransformData> t_transformDatas{};

			for (const auto& t_entry : t_scene) {
				bool t_hasLight = false;
				bool t_hasRigidBody = false;
				bool t_hasMesh = false;
				bool t_hasCamera = false;
				bool t_hasPlayerController = false;

				std::string t_name = t_entry.at("GameObject").get<std::string>();

				TransformData t_transform = DeserializeTransformComponent(t_entry.at("TransformComponent"));
				t_transformDatas.emplace_back(t_transform);

				LightData t_light{};
				if (t_entry.contains("LightComponent"))
				{
					t_light = DeserializeLightComponent(t_entry.at("LightComponent"));
					t_hasLight = true;
				}

				RigidBodyData t_rigidBody{};
				if (t_entry.contains("RigidBodyComponent")) 
				{
					t_rigidBody = DeserializeRigidBodyComponent(t_entry.at("RigidBodyComponent"));
					t_hasRigidBody = true;
				}

				MeshData t_mesh{};
				if (t_entry.contains("MeshComponent"))
				{
					t_mesh = DeserializeMeshComponent(t_entry.at("MeshComponent"));
					t_hasMesh = true;
				}

				CameraSerializeData t_camera{};
				if (t_entry.contains("CameraComponent"))
				{
					t_camera = DeserializeCameraComponent(t_entry.at("CameraComponent"));
					t_hasCamera = true;
				}

				PlayerControllerData t_playerController{};
				if (t_entry.contains("PlayerControllerComponent"))
				{
					t_playerController = DeserializePlayerControllerComponent(t_entry.at("PlayerControllerComponent"));
					t_hasPlayerController = true;
				}

				GameObject* t_gameObject = new GameObject(t_transform.mPos, t_transform.mRot, t_transform.mScale, t_name);
				AddGameObject(t_gameObject);

				for (auto& t_compEntry : ComponentRegistry::Instance().GetEntries())
				{
					if (t_entry.contains(t_compEntry.name))
					{
						t_compEntry.addFunction(*t_gameObject);

						t_compEntry.getComponent(*t_gameObject)->Deserialize(t_entry.at(t_compEntry.name));
					}
				}

				if (t_hasLight)
				{
					t_gameObject->AddComponent<LightComponent>();
					t_gameObject->GetComponent<LightComponent>()->SetLightFromData(t_light);
				}

				if (t_hasRigidBody)
				{
					t_gameObject->AddComponent<RigidBodyComponent>();
					RigidBodyComponent* t_rigidBodyComponent = t_gameObject->GetComponent<RigidBodyComponent>();
					Physics::BodyType t_bodyType = static_cast<Physics::BodyType>(t_rigidBody.mBodyType);
					Physics::CollisionLayer t_layer = static_cast<Physics::CollisionLayer>(t_rigidBody.mLayer);
					Math::vec3 t_pos = t_rigidBody.mPos;
					Math::vec3 t_scale = t_rigidBody.mScale;
					float t_radius = t_rigidBody.mRadius;
					float t_halfHeight = t_rigidBody.mHalfHeight;
					Math::quat t_rot = t_rigidBody.mRot;
					float t_mass = t_rigidBody.mMass;
					bool t_enable = t_rigidBody.mEnable;
					bool t_lockRotX = t_rigidBody.mLockRotX;
					bool t_lockRotY = t_rigidBody.mLockRotY;
					bool t_lockRotZ = t_rigidBody.mLockRotZ;
					float t_friction = t_rigidBody.mFriction;
					float t_restitution = t_rigidBody.mRestitution;
					Math::Transform t_transform = *t_gameObject->GetComponent<TransformComponent>()->GetTransform();

					switch (t_rigidBody.mColliderType)
					{
					case 0:
						t_rigidBodyComponent->CreateBoxRigidBody(t_bodyType, t_layer, t_pos, t_scale, t_rot, t_transform, t_mass, t_enable);
						break;
					case 1:
						t_rigidBodyComponent->CreateSphereRigidBody(t_bodyType, t_layer, t_pos, t_radius, t_rot, t_transform, t_mass, t_enable);
						break;
					case 2:
						t_rigidBodyComponent->CreateCapsuleRigidBody(t_bodyType, t_layer, t_pos, t_halfHeight, t_radius, t_rot, t_transform, t_mass, t_enable);
						break;
					}

					if (t_lockRotX)
					{
						t_rigidBodyComponent->LockRotation('x');
					}
					if (t_lockRotY)
					{
						t_rigidBodyComponent->LockRotation('y');
					}
					if (t_lockRotZ)
					{
						t_rigidBodyComponent->LockRotation('z');
					}

					if (t_friction)
					{
						t_rigidBodyComponent->GetBody()->SetFriction(t_friction);
					}

					if (t_restitution)
					{
						t_rigidBodyComponent->GetBody()->SetRestitution(t_restitution);
					}
				}

				if (t_hasMesh)
				{
					t_gameObject->AddComponent<MeshComponent>();
					MeshComponent* t_meshComponent = t_gameObject->GetComponent<MeshComponent>();
					t_meshComponent->SetMesh(t_mesh.mMeshPath, a_renderer);
					Ref<Material> t_material = t_meshComponent->GetMaterial();
					t_material->Create(static_cast<MaterialType>(t_mesh.mType));
					if (!t_mesh.mAlbedoPath.empty())
					{
						t_material->SetAlbedo(t_mesh.mAlbedoPath, a_renderer);
					}
					if (!t_mesh.mNormalPath.empty())
					{
						t_material->SetNormal(t_mesh.mNormalPath, a_renderer);
					}
					if (!t_mesh.mMetallicPath.empty())
					{
						t_material->SetMetallic(t_mesh.mMetallicPath, a_renderer);
					}
					if (!t_mesh.mRoughnessPath.empty())
					{
						t_material->SetRoughness(t_mesh.mRoughnessPath, a_renderer);
					}
					if (!t_mesh.mAOPath.empty())
					{
						t_material->SetAO(t_mesh.mAOPath, a_renderer);
					}
				}

				if (t_hasCamera)
				{
					t_gameObject->AddComponent<CameraComponent>();
					t_gameObject->GetComponent<CameraComponent>()->GetCamera().Set(t_camera.m_up, t_camera.m_center, t_camera.m_eye, t_camera.m_fovY, t_camera.m_aspect, t_camera.m_near, t_camera.m_far, t_camera.m_speed, t_camera.m_sensitivity, false, t_gameObject->GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix());
					t_gameObject->GetComponent<CameraComponent>()->GetCamera().Create(a_renderer);
					SetMainCamera(t_gameObject->GetId());
				}
				if (t_hasPlayerController)
				{
					t_gameObject->AddComponent<PlayerControllerComponent>();
					t_gameObject->GetComponent<PlayerControllerComponent>()->Set(t_playerController);
				}


			}

			for (uint32_t i = 0; i < t_transformDatas.size(); ++i)
			{
				TransformComponent* t_transformComponent = GetGameObject(i)->GetComponent<TransformComponent>();
				m_transformSystem->Update(); // Try something
				t_transformComponent->SetParent(t_transformDatas[i].mParentId);
			}
			t_transformDatas.clear();
		}

		void Scene::Reload(Core::Renderer* a_renderer)
		{
			m_lastState = m_isPlaying;
			for (int i = 0; i < m_objs.size(); ++i)
			{
				if (!m_objs[i])
				{
					continue;
				}
				delete m_objs[i];
			}
			m_objs.clear();

			m_renderSystem->Destroy(a_renderer);
			m_renderSystem->Create(a_renderer);
			m_physicsSystem->RemoveAllComponents();
			m_transformSystem->Destroy();
			Load(a_renderer);
			m_justReloaded = true;
		}
		
		bool Scene::SetMode(bool a_mode)
		{
			bool sucess = true;
			if (a_mode && !m_renderSystem->GetCameraDescriptor(m_gameCameraId))
			{
				sucess = false;
				return sucess;
			}
			m_isPlaying = a_mode;
			if (m_isPlaying)
			{
				m_beginPlayEvent.Invoke();
			}
			else
			{
				m_endPlayEvent.Invoke();
			}
			return sucess;
		}

		bool Scene::HasObject(int a_id)
		{
			for (GameObject* t_object : m_objs)
			{
				if (!t_object)
					continue;
				if (t_object->GetId() == a_id)
				{
					return true;
				}
			}
			return false;
		}

		nlohmann::ordered_json Scene::SerializeTransformComponent(const TransformComponent& a_transform, std::vector<std::pair<int, int>> a_oldAndNewIndexes)
		{
			json t_json;
			constexpr std::hash<std::string_view> t_hash{};

			meta::any t_transformAny{ a_transform };

			const meta::handle t_transformHandle{ t_transformAny };

			if (!t_transformHandle)
			{
				return t_json;
			}

			const meta::data t_transformDataField = t_transformHandle.type().data(t_hash("Transform"));
			if (!t_transformDataField)
			{
				return t_json;
			}

			meta::any t_transformDataAny = t_transformDataField.get(t_transformHandle);
			if (!t_transformDataAny)
			{
				return t_json;
			}

			const meta::handle t_transformDataHandle(t_transformDataAny);

			const meta::type t_transformDataType = t_transformDataHandle.type();

			const meta::data t_posField = t_transformDataType.data(t_hash("position"));
			if (t_posField)
			{
				meta::any t_posAny = t_posField.get(t_transformDataHandle);
				const meta::handle t_posHandle(t_posAny);

				t_json["position"] = {
					{"x", t_posHandle.type().data(t_hash("x")).get(t_posHandle).cast<float>()},
					{"y", t_posHandle.type().data(t_hash("y")).get(t_posHandle).cast<float>()},
					{"z", t_posHandle.type().data(t_hash("z")).get(t_posHandle).cast<float>()}
				};
			}

			const meta::data t_rotField = t_transformDataType.data(t_hash("rotation"));
			if (t_rotField)
			{
				meta::any t_rotAny = t_rotField.get(t_transformDataHandle);
				const meta::handle t_rotHandle(t_rotAny);

				t_json["rotation"] = {
					{"x", t_rotHandle.type().data(t_hash("x")).get(t_rotHandle).cast<float>()},
					{"y", t_rotHandle.type().data(t_hash("y")).get(t_rotHandle).cast<float>()},
					{"z", t_rotHandle.type().data(t_hash("z")).get(t_rotHandle).cast<float>()},
					{"w", t_rotHandle.type().data(t_hash("w")).get(t_rotHandle).cast<float>()}
				};
			}

			const meta::data t_scaleField = t_transformDataType.data(t_hash("scale"));
			if (t_scaleField)
			{
				meta::any t_scaleAny = t_scaleField.get(t_transformDataHandle);
				const meta::handle t_scaleHandle(t_scaleAny);

				t_json["scale"] = {
					{"x", t_scaleHandle.type().data(t_hash("x")).get(t_scaleHandle).cast<float>()},
					{"y", t_scaleHandle.type().data(t_hash("y")).get(t_scaleHandle).cast<float>()},
					{"z", t_scaleHandle.type().data(t_hash("z")).get(t_scaleHandle).cast<float>()}
				};
			}

			const meta::data t_parentField = t_transformDataType.data(t_hash("parent"));
			if (t_parentField)
			{
				meta::any t_parentAny = t_parentField.get(t_transformDataHandle);


				if (t_parentAny.cast<int>() != -1)
				{
					for (int i = 0; i < a_oldAndNewIndexes.size(); ++i)
					{
						if (a_oldAndNewIndexes[i].first == t_parentAny.cast<int>())
						{
							t_parentAny = a_oldAndNewIndexes[i].second;
						}
					}
				}
				t_json["parent"] = t_parentAny.cast<int>();
			}

			return t_json;
		}

		TransformData Scene::DeserializeTransformComponent(const nlohmann::ordered_json& a_json)
		{
			TransformData t_outData;

			const auto& t_pos = a_json.at("position");
			t_outData.mPos = {
				t_pos.at("x").get<float>(),
				t_pos.at("y").get<float>(),
				t_pos.at("z").get<float>()
			};

			const auto& t_rot = a_json.at("rotation");
			t_outData.mRot = {
				t_rot.at("x").get<float>(),
				t_rot.at("y").get<float>(),
				t_rot.at("z").get<float>(),
				t_rot.at("w").get<float>()
			};

			const auto& t_scale = a_json.at("scale");
			t_outData.mScale = {
				t_scale.at("x").get<float>(),
				t_scale.at("y").get<float>(),
				t_scale.at("z").get<float>()
			};

			t_outData.mParentId = a_json.at("parent").get<int>();

			return t_outData;
		}

		nlohmann::ordered_json Scene::SerializeRigidBodyComponent(const RigidBodyComponent& a_rigidBody)
		{
			json t_json;
			constexpr std::hash<std::string_view> t_hash{};

			meta::any t_rigidBodyAny{ a_rigidBody };

			const meta::handle t_rigidBodyHandle{ t_rigidBodyAny };

			if (!t_rigidBodyHandle)
			{
				return t_json;
			}

			const meta::data t_rigidBodyDataField = t_rigidBodyHandle.type().data(t_hash("RigidBody"));
			if (!t_rigidBodyDataField)
			{
				return t_json;
			}

			meta::any t_rigidBodyDataAny = t_rigidBodyDataField.get(t_rigidBodyHandle);
			if (!t_rigidBodyDataAny)
			{
				return t_json;
			}

			const meta::handle t_rigidBodyDataHandle(t_rigidBodyDataAny);

			const meta::type t_rigidBodyDataType = t_rigidBodyDataHandle.type();

			const meta::data t_bodyTypeField = t_rigidBodyDataType.data(t_hash("body type"));
			if (t_bodyTypeField)
			{
				meta::any t_bodyTypeAny = t_bodyTypeField.get(t_rigidBodyDataHandle);
				t_json["body type"] = t_bodyTypeAny.cast<int>();
			}

			const meta::data t_layerField = t_rigidBodyDataType.data(t_hash("layer"));
			if (t_layerField)
			{
				meta::any t_layerAny = t_layerField.get(t_rigidBodyDataHandle);
				t_json["layer"] = t_layerAny.cast<int>();
			}

			const meta::data t_colliderTypeField = t_rigidBodyDataType.data(t_hash("collider type"));
			if (t_colliderTypeField)
			{
				meta::any t_colliderTypeAny = t_colliderTypeField.get(t_rigidBodyDataHandle);
				t_json["collider type"] = t_colliderTypeAny.cast<int>();
			}

			const meta::data t_posField = t_rigidBodyDataType.data(t_hash("position"));
			if (t_posField)
			{
				meta::any t_posAny = t_posField.get(t_rigidBodyDataHandle);
				const meta::handle t_posHandle(t_posAny);

				t_json["position"] = {
					{"x", t_posHandle.type().data(t_hash("x")).get(t_posHandle).cast<float>()},
					{"y", t_posHandle.type().data(t_hash("y")).get(t_posHandle).cast<float>()},
					{"z", t_posHandle.type().data(t_hash("z")).get(t_posHandle).cast<float>()}
				};
			}

			const meta::data t_scaleField = t_rigidBodyDataType.data(t_hash("scale"));
			if (t_scaleField)
			{
				meta::any t_scaleAny = t_scaleField.get(t_rigidBodyDataHandle);
				const meta::handle t_scaleHandle(t_scaleAny);

				t_json["scale"] = {
					{"x", t_scaleHandle.type().data(t_hash("x")).get(t_scaleHandle).cast<float>()},
					{"y", t_scaleHandle.type().data(t_hash("y")).get(t_scaleHandle).cast<float>()},
					{"z", t_scaleHandle.type().data(t_hash("z")).get(t_scaleHandle).cast<float>()}
				};
			}

			const meta::data t_radiusTypeField = t_rigidBodyDataType.data(t_hash("radius"));
			if (t_radiusTypeField)
			{
				meta::any t_radiusTypeAny = t_radiusTypeField.get(t_rigidBodyDataHandle);
				t_json["radius"] = t_radiusTypeAny.cast<float>();
			}

			const meta::data t_halfHeightTypeField = t_rigidBodyDataType.data(t_hash("half height"));
			if (t_halfHeightTypeField)
			{
				meta::any t_halfHeightTypeAny = t_halfHeightTypeField.get(t_rigidBodyDataHandle);
				t_json["half height"] = t_halfHeightTypeAny.cast<float>();
			}

			const meta::data t_rotField = t_rigidBodyDataType.data(t_hash("rotation"));
			if (t_rotField)
			{
				meta::any t_rotAny = t_rotField.get(t_rigidBodyDataHandle);
				const meta::handle t_rotHandle(t_rotAny);

				t_json["rotation"] = {
					{"x", t_rotHandle.type().data(t_hash("x")).get(t_rotHandle).cast<float>()},
					{"y", t_rotHandle.type().data(t_hash("y")).get(t_rotHandle).cast<float>()},
					{"z", t_rotHandle.type().data(t_hash("z")).get(t_rotHandle).cast<float>()},
					{"w", t_rotHandle.type().data(t_hash("w")).get(t_rotHandle).cast<float>()}
				};
			}

			const meta::data t_massTypeField = t_rigidBodyDataType.data(t_hash("mass"));
			if (t_massTypeField)
			{
				meta::any t_massTypeAny = t_massTypeField.get(t_rigidBodyDataHandle);
				t_json["mass"] = t_massTypeAny.cast<float>();
			}

			const meta::data t_enableTypeField = t_rigidBodyDataType.data(t_hash("enable"));
			if (t_enableTypeField)
			{
				meta::any t_enableTypeAny = t_enableTypeField.get(t_rigidBodyDataHandle);
				t_json["enable"] = t_enableTypeAny.cast<bool>();
			}

			const meta::data t_lockRotXTypeField = t_rigidBodyDataType.data(t_hash("lock rotation X"));
			if (t_lockRotXTypeField)
			{
				meta::any t_lockRotXTypeAny = t_lockRotXTypeField.get(t_rigidBodyDataHandle);
				t_json["lock rotation X"] = t_lockRotXTypeAny.cast<bool>();
			}

			const meta::data t_lockRotYTypeField = t_rigidBodyDataType.data(t_hash("lock rotation Y"));
			if (t_lockRotYTypeField)
			{
				meta::any t_lockRotYTypeAny = t_lockRotYTypeField.get(t_rigidBodyDataHandle);
				t_json["lock rotation Y"] = t_lockRotYTypeAny.cast<bool>();
			}

			const meta::data t_lockRotZTypeField = t_rigidBodyDataType.data(t_hash("lock rotation Z"));
			if (t_lockRotZTypeField)
			{
				meta::any t_lockRotZTypeAny = t_lockRotZTypeField.get(t_rigidBodyDataHandle);
				t_json["lock rotation Z"] = t_lockRotZTypeAny.cast<bool>();
			}

			const meta::data t_frictionTypeField = t_rigidBodyDataType.data(t_hash("friction"));
			if (t_frictionTypeField)
			{
				meta::any t_frictionTypeAny = t_frictionTypeField.get(t_rigidBodyDataHandle);
				t_json["friction"] = t_frictionTypeAny.cast<float>();
			}

			const meta::data t_restitutionTypeField = t_rigidBodyDataType.data(t_hash("restitution"));
			if (t_restitutionTypeField)
			{
				meta::any t_restitutionTypeAny = t_restitutionTypeField.get(t_rigidBodyDataHandle);
				t_json["restitution"] = t_restitutionTypeAny.cast<float>();
			}

			return t_json;
		}

		RigidBodyData Scene::DeserializeRigidBodyComponent(const nlohmann::ordered_json& a_json)
		{
			RigidBodyData t_outData;

			t_outData.mBodyType = a_json.at("body type").get<int>();

			t_outData.mLayer = a_json.at("layer").get<int>();

			t_outData.mColliderType = a_json.at("collider type").get<int>();

			const auto& t_pos = a_json.at("position");
			t_outData.mPos = {
				t_pos.at("x").get<float>(),
				t_pos.at("y").get<float>(),
				t_pos.at("z").get<float>()
			};

			const auto& t_scale = a_json.at("scale");
			t_outData.mScale = {
				t_scale.at("x").get<float>(),
				t_scale.at("y").get<float>(),
				t_scale.at("z").get<float>()
			};

			t_outData.mRadius = a_json.at("radius").get<float>();

			t_outData.mHalfHeight = a_json.at("half height").get<float>();

			const auto& t_rot = a_json.at("rotation");
			t_outData.mRot = {
				t_rot.at("x").get<float>(),
				t_rot.at("y").get<float>(),
				t_rot.at("z").get<float>(),
				t_rot.at("w").get<float>()
			};

			t_outData.mMass = a_json.at("mass").get<float>();

			t_outData.mEnable = a_json.at("enable").get<bool>();

			t_outData.mLockRotX = a_json.at("lock rotation X").get<bool>();

			t_outData.mLockRotY = a_json.at("lock rotation Y").get<bool>();

			t_outData.mLockRotZ = a_json.at("lock rotation Z").get<bool>();

			t_outData.mFriction = a_json.at("friction").get<float>();

			t_outData.mRestitution = a_json.at("restitution").get<float>();

			return t_outData;
		}

		nlohmann::ordered_json Scene::SerializeMeshComponent(const MeshComponent& a_mesh)
		{
			json t_json;
			constexpr std::hash<std::string_view> t_hash{};

			meta::any t_meshAny{ a_mesh };

			const meta::handle t_meshHandle{ t_meshAny };

			if (!t_meshHandle)
			{
				return t_json;
			}

			const meta::data t_meshDataField = t_meshHandle.type().data(t_hash("Mesh"));
			if (!t_meshDataField)
			{
				return t_json;
			}

			meta::any t_meshDataAny = t_meshDataField.get(t_meshHandle);
			if (!t_meshDataAny)
			{
				return t_json;
			}

			const meta::handle t_meshDataHandle(t_meshDataAny);

			const meta::type t_meshDataType = t_meshDataHandle.type();

			const meta::data t_meshField = t_meshDataType.data(t_hash("mesh"));
			if (t_meshField)
			{
				meta::any t_meshAny = t_meshField.get(t_meshDataHandle);
				t_json["mesh"] = t_meshAny.cast<std::string>();
			}

			const meta::data t_typeField = t_meshDataType.data(t_hash("type"));
			if (t_typeField)
			{
				meta::any t_typeAny = t_typeField.get(t_meshDataHandle);
				t_json["type"] = t_typeAny.cast<int>();
			}

			const meta::data t_albedoField = t_meshDataType.data(t_hash("albedo"));
			if (t_albedoField)
			{
				meta::any t_albedoAny = t_albedoField.get(t_meshDataHandle);
				t_json["albedo"] = t_albedoAny.cast<std::string>();
			}

			const meta::data t_normalField = t_meshDataType.data(t_hash("normal"));
			if (t_normalField)
			{
				meta::any t_normalAny = t_normalField.get(t_meshDataHandle);
				t_json["normal"] = t_normalAny.cast<std::string>();
			}

			const meta::data t_metallicField = t_meshDataType.data(t_hash("metallic"));
			if (t_metallicField)
			{
				meta::any t_metallicAny = t_metallicField.get(t_meshDataHandle);
				t_json["metallic"] = t_metallicAny.cast<std::string>();
			}

			const meta::data t_roughnessField = t_meshDataType.data(t_hash("roughness"));
			if (t_roughnessField)
			{
				meta::any t_roughnessAny = t_roughnessField.get(t_meshDataHandle);
				t_json["roughness"] = t_roughnessAny.cast<std::string>();
			}

			const meta::data t_AOField = t_meshDataType.data(t_hash("ao"));
			if (t_AOField)
			{
				meta::any t_AOAny = t_AOField.get(t_meshDataHandle);
				t_json["ao"] = t_AOAny.cast<std::string>();
			}

			return t_json;
		}

		MeshData Scene::DeserializeMeshComponent(const nlohmann::ordered_json& a_json)
		{
			MeshData t_meshData;

			t_meshData.mMeshPath = a_json.at("mesh").get<std::string>();

			t_meshData.mType = a_json.at("type").get<int>();

			t_meshData.mAlbedoPath = a_json.at("albedo").get<std::string>();

			t_meshData.mNormalPath = a_json.at("normal").get<std::string>();

			t_meshData.mMetallicPath = a_json.at("metallic").get<std::string>();

			t_meshData.mRoughnessPath = a_json.at("roughness").get<std::string>();

			t_meshData.mAOPath = a_json.at("ao").get<std::string>();

			return t_meshData;
		}

		nlohmann::ordered_json Scene::SerializeLightComponent(const LightComponent& a_lightComponent)
		{
			json t_json;
			constexpr std::hash<std::string_view> t_hash{};

			meta::any t_lightAny{ a_lightComponent };

			const meta::handle t_lightHandle{ t_lightAny };

			if (!t_lightHandle)
			{
				return t_json;
			}

			const meta::data t_lightDataField = t_lightHandle.type().data(t_hash("Light"));
			if (!t_lightDataField)
			{
				return t_json;
			}

			meta::any t_lightDataAny = t_lightDataField.get(t_lightHandle);
			if (!t_lightDataAny)
			{
				return t_json;
			}

			const meta::handle t_lightDataHandle(t_lightDataAny);

			const meta::type t_lightDataType = t_lightDataHandle.type();

			const meta::data t_posField = t_lightDataType.data(t_hash("position"));
			if (t_posField)
			{
				meta::any t_posAny = t_posField.get(t_lightDataHandle);
				const meta::handle t_posHandle(t_posAny);

				t_json["position"] = {
					{"x", t_posHandle.type().data(t_hash("x")).get(t_posHandle).cast<float>()},
					{"y", t_posHandle.type().data(t_hash("y")).get(t_posHandle).cast<float>()},
					{"z", t_posHandle.type().data(t_hash("z")).get(t_posHandle).cast<float>()}
				};
			}

			const meta::data t_colorField = t_lightDataType.data(t_hash("color"));
			if (t_colorField)
			{
				meta::any t_scaleAny = t_colorField.get(t_lightDataHandle);
				const meta::handle t_scaleHandle(t_scaleAny);

				t_json["color"] = {
					{"x", t_scaleHandle.type().data(t_hash("x")).get(t_scaleHandle).cast<float>()},
					{"y", t_scaleHandle.type().data(t_hash("y")).get(t_scaleHandle).cast<float>()},
					{"z", t_scaleHandle.type().data(t_hash("z")).get(t_scaleHandle).cast<float>()}
				};
			}

			const meta::data t_intensityField = t_lightDataType.data(t_hash("intensity"));
			if (t_intensityField)
			{
				meta::any t_intensityAny = t_intensityField.get(t_lightDataHandle);
				t_json["intensity"] = t_intensityAny.cast<float>();
			}

			const meta::data t_isDirField = t_lightDataType.data(t_hash("is dir"));
			if (t_isDirField)
			{
				meta::any t_isDirAny = t_isDirField.get(t_lightDataHandle);
				t_json["is dir"] = t_isDirAny.cast<uint32_t>();
			}

			return t_json;
		}

		LightData Scene::DeserializeLightComponent(const nlohmann::ordered_json& a_json)
		{
			LightData t_lightData;

			const auto& t_pos = a_json.at("position");
			t_lightData.m_position = {
				t_pos.at("x").get<float>(),
				t_pos.at("y").get<float>(),
				t_pos.at("z").get<float>()
			};

			const auto& t_color = a_json.at("color");
			t_lightData.m_color = {
				t_color.at("x").get<float>(),
				t_color.at("y").get<float>(),
				t_color.at("z").get<float>()
			};

			t_lightData.m_intensity = a_json.at("intensity").get<float>();

			t_lightData.m_bDir = a_json.at("is dir").get<uint32_t>();

			return t_lightData;
		}

		nlohmann::ordered_json Scene::SerializeCameraComponent(const CameraComponent& a_cameraComponent)
		{
			json t_json;
			constexpr std::hash<std::string_view> t_hash{};

			meta::any t_cameraAny{ a_cameraComponent };

			const meta::handle t_cameraHandle{ t_cameraAny };

			if (!t_cameraHandle)
			{
				return t_json;
			}

			const meta::data t_cameraDataField = t_cameraHandle.type().data(t_hash("Camera"));
			if (!t_cameraDataField)
			{
				return t_json;
			}

			meta::any t_cameraDataAny = t_cameraDataField.get(t_cameraHandle);
			if (!t_cameraDataAny)
			{
				return t_json;
			}

			const meta::handle t_cameraDataHandle(t_cameraDataAny);

			const meta::type t_cameraDataType = t_cameraDataHandle.type();

			const meta::data t_upField = t_cameraDataType.data(t_hash("up"));
			if (t_upField)
			{
				meta::any t_upAny = t_upField.get(t_cameraDataHandle);
				const meta::handle t_upHandle(t_upAny);

				t_json["up"] = {
					{"x", t_upHandle.type().data(t_hash("x")).get(t_upHandle).cast<float>()},
					{"y", t_upHandle.type().data(t_hash("y")).get(t_upHandle).cast<float>()},
					{"z", t_upHandle.type().data(t_hash("z")).get(t_upHandle).cast<float>()}
				};
			}

			const meta::data t_centerField = t_cameraDataType.data(t_hash("center"));
			if (t_centerField)
			{
				meta::any t_centerAny = t_centerField.get(t_cameraDataHandle);
				const meta::handle t_centerHandle(t_centerAny);

				t_json["center"] = {
					{"x", t_centerHandle.type().data(t_hash("x")).get(t_centerHandle).cast<float>()},
					{"y", t_centerHandle.type().data(t_hash("y")).get(t_centerHandle).cast<float>()},
					{"z", t_centerHandle.type().data(t_hash("z")).get(t_centerHandle).cast<float>()}
				};
			}

			const meta::data t_eyeField = t_cameraDataType.data(t_hash("eye"));
			if (t_eyeField)
			{
				meta::any t_eyeAny = t_eyeField.get(t_cameraDataHandle);
				const meta::handle t_eyeHandle(t_eyeAny);

				t_json["eye"] = {
					{"x", t_eyeHandle.type().data(t_hash("x")).get(t_eyeHandle).cast<float>()},
					{"y", t_eyeHandle.type().data(t_hash("y")).get(t_eyeHandle).cast<float>()},
					{"z", t_eyeHandle.type().data(t_hash("z")).get(t_eyeHandle).cast<float>()}
				};
			}

			const meta::data t_fovYField = t_cameraDataType.data(t_hash("fovY"));
			if (t_fovYField)
			{
				meta::any t_fovYAny = t_fovYField.get(t_cameraDataHandle);
				t_json["fovY"] = t_fovYAny.cast<float>();
			}

			const meta::data t_aspectField = t_cameraDataType.data(t_hash("aspect"));
			if (t_aspectField)
			{
				meta::any t_aspectYAny = t_aspectField.get(t_cameraDataHandle);
				t_json["aspect"] = t_aspectYAny.cast<float>();
			}

			const meta::data t_nearField = t_cameraDataType.data(t_hash("near"));
			if (t_nearField)
			{
				meta::any t_nearYAny = t_nearField.get(t_cameraDataHandle);
				t_json["near"] = t_nearYAny.cast<float>();
			}

			const meta::data t_farField = t_cameraDataType.data(t_hash("far"));
			if (t_farField)
			{
				meta::any t_farYAny = t_farField.get(t_cameraDataHandle);
				t_json["far"] = t_farYAny.cast<float>();
			}

			const meta::data t_speedField = t_cameraDataType.data(t_hash("speed"));
			if (t_speedField)
			{
				meta::any t_speedYAny = t_speedField.get(t_cameraDataHandle);
				t_json["speed"] = t_speedYAny.cast<float>();
			}

			const meta::data t_sensitivityField = t_cameraDataType.data(t_hash("sensitivity"));
			if (t_sensitivityField)
			{
				meta::any t_sensitivityYAny = t_sensitivityField.get(t_cameraDataHandle);
				t_json["sensitivity"] = t_sensitivityYAny.cast<float>();
			}

			return t_json;
		}

		CameraSerializeData Scene::DeserializeCameraComponent(const nlohmann::ordered_json& a_json)
		{
			CameraSerializeData t_cameraData;

			const auto& t_up = a_json.at("up");
			t_cameraData.m_up = {
				t_up.at("x").get<float>(),
				t_up.at("y").get<float>(),
				t_up.at("z").get<float>()
			};

			const auto& t_center = a_json.at("up");
			t_cameraData.m_center = {
				t_center.at("x").get<float>(),
				t_center.at("y").get<float>(),
				t_center.at("z").get<float>()
			};

			const auto& t_eye = a_json.at("up");
			t_cameraData.m_eye = {
				t_eye.at("x").get<float>(),
				t_eye.at("y").get<float>(),
				t_eye.at("z").get<float>()
			};

			t_cameraData.m_fovY = a_json.at("fovY").get<float>();
			t_cameraData.m_aspect = a_json.at("aspect").get<float>();
			t_cameraData.m_near = a_json.at("near").get<float>();
			t_cameraData.m_far = a_json.at("far").get<float>();
			t_cameraData.m_speed = a_json.at("speed").get<float>();
			t_cameraData.m_sensitivity = a_json.at("sensitivity").get<float>();

			return t_cameraData;
		}

		nlohmann::ordered_json Scene::SerializePlayerControllerComponent(const PlayerControllerComponent& a_gameObject)
		{
			json t_json;
			constexpr std::hash<std::string_view> t_hash{};
			meta::any t_playerControllerAny{ a_gameObject };
			const meta::handle t_playerControllerHandle{ t_playerControllerAny };
			if (!t_playerControllerHandle)
			{
				return t_json;
			}
			const meta::data t_playerControllerDataField = t_playerControllerHandle.type().data(t_hash("PlayerController"));
			if (!t_playerControllerDataField)
			{
				return t_json;
			}
			meta::any t_playerControllerDataAny = t_playerControllerDataField.get(t_playerControllerHandle);
			if (!t_playerControllerDataAny)
			{
				return t_json;
			}
			const meta::handle t_playerControllerDataHandle(t_playerControllerDataAny);
			const meta::type t_playerControllerDataType = t_playerControllerDataHandle.type();

			const meta::data t_maxSpeedField = t_playerControllerDataType.data(t_hash("maxSpeed"));
			if (t_maxSpeedField)
			{
				meta::any t_speedAny = t_maxSpeedField.get(t_playerControllerDataHandle);
				t_json["maxSpeed"] = t_speedAny.cast<float>();
			}
			const meta::data t_sensitivityField = t_playerControllerDataType.data(t_hash("sensitivity"));
			if (t_sensitivityField)
			{
				meta::any t_sensitivityAny = t_sensitivityField.get(t_playerControllerDataHandle);
				t_json["sensitivity"] = t_sensitivityAny.cast<float>();
			}
			const meta::data t_jumpForceField = t_playerControllerDataType.data(t_hash("jumpForce"));
			if (t_jumpForceField)
			{
				meta::any t_jumpForceAny = t_jumpForceField.get(t_playerControllerDataHandle);
				t_json["jumpForce"] = t_jumpForceAny.cast<float>();
			}
			const meta::data t_moveSpeedField = t_playerControllerDataType.data(t_hash("moveSpeed"));
			if (t_moveSpeedField)
			{
				meta::any t_moveSpeedAny = t_moveSpeedField.get(t_playerControllerDataHandle);
				t_json["moveSpeed"] = t_moveSpeedAny.cast<float>();
			}
			const meta::data t_maxUpAngle = t_playerControllerDataType.data(t_hash("maxUpAngle"));
			if (t_maxUpAngle)
			{
				meta::any t_maxUpAngleAny = t_maxUpAngle.get(t_playerControllerDataHandle);
				t_json["maxUpAngle"] = t_maxUpAngleAny.cast<float>();
			}
			const meta::data t_maxDownAngle = t_playerControllerDataType.data(t_hash("maxDownAngle"));
			if (t_maxDownAngle)
			{
				meta::any t_maxDownAngleAny = t_maxDownAngle.get(t_playerControllerDataHandle);
				t_json["maxDownAngle"] = t_maxDownAngleAny.cast<float>();
			}
			const meta::data t_transformRotateComponentId = t_playerControllerDataType.data(t_hash("transformRotateComponentId"));
			if (t_transformRotateComponentId)
			{
				meta::any t_transformRotateComponentIdAny = t_transformRotateComponentId.get(t_playerControllerDataHandle);
				t_json["transformRotateComponentId"] = t_transformRotateComponentIdAny.cast<int>();
			}
			return t_json;
		}

		PlayerControllerData Scene::DeserializePlayerControllerComponent(const nlohmann::ordered_json& a_json)
		{
			PlayerControllerData t_playerControllerData;
			t_playerControllerData.m_maxSpeed = a_json.at("maxSpeed").get<float>();
			t_playerControllerData.m_sensitivity = a_json.at("sensitivity").get<float>();
			t_playerControllerData.m_jumpForce = a_json.at("jumpForce").get<float>();
			t_playerControllerData.m_moveSpeed = a_json.at("moveSpeed").get<float>();
			t_playerControllerData.m_maxUpAngle = a_json.at("maxUpAngle").get<float>();
			t_playerControllerData.m_maxDownAngle = a_json.at("maxDownAngle").get<float>();
			t_playerControllerData.m_transformRotateComponentId = a_json.at("transformRotateComponentId").get<int>();
			return t_playerControllerData;
		}
	}
}
