#include "GamePlay/Others/Scene.h"

#include <utility>
#include <meta/factory.hpp>
#include <meta/meta.hpp>

#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Others/LightGO.h"
#include "Math/TheCoolerMath.h"

using json = nlohmann::ordered_json;

#include "Core/GraphicsAPI/Vulkan/VulkanShader.h"
namespace Engine
{
	namespace GamePlay
	{
		void Scene::Create(Core::Renderer* a_renderer, const char* a_name, int a_width, int a_height)
		{
			m_name = a_name;

			m_transformSystem = new TransformSystem;

			m_renderSystem = new RenderSystem;

			m_physicsSystem = new PhysicsSystem;
			m_physicsSystem->Create();

			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();
			ServiceLocator::ProvideTransformSystem(m_transformSystem);
			ServiceLocator::ProvideRenderSystem(m_renderSystem);
			ServiceLocator::ProvidePhysicsSystem(m_physicsSystem);

			m_mainCamera = new Camera(Math::vec3(0.f, 1.f, 0.f), Math::vec3(0.f, 0.f, 0.f),
				Math::vec3(0.f, 1.f, 3.f), Math::ToRadians(70.f),
				static_cast<float>(a_width) / static_cast<float>(a_height), 0.1f, 100.f, 10.f, 20.f);
			m_mainCamera->Create(a_renderer);

			GameObject* t_object = new GameObject(Math::vec3(0.f, 0.f, 0.f), Math::vec3(0.f, Math::ToRadians(270.f), Math::ToRadians(270.f)), Math::vec3(1.f), "Unlit");
			t_object->AddComponent<MeshComponent>();
			t_object->GetComponent<MeshComponent>()->SetMesh("Assets/Meshes/viking_room.obj", a_renderer);
			t_object->GetComponent<MeshComponent>()->GetMaterial()->SetType(UNLIT);
			t_object->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo("Assets/Textures/viking_room.png", a_renderer);

			GameObject* t_object2 = new GameObject(Math::vec3(5.f, 0.f, 0.f), Math::vec3(0.f, Math::ToRadians(270.f), Math::ToRadians(270.f)), Math::vec3(1.f), "Lit");
			t_object2->AddComponent<MeshComponent>();
			t_object2->GetComponent<MeshComponent>()->SetMesh("Assets/Meshes/viking_room.obj", a_renderer);
			t_object2->GetComponent<MeshComponent>()->GetMaterial()->SetType(LIT);
			t_object2->GetComponent<MeshComponent>()->GetMaterial()->SetAlbedo("Assets/Textures/viking_room.png", a_renderer);

			LightGO* t_light = new LightGO(Math::vec3(10.f, 0.f, 0.f), Math::vec3(0.f, 0.f, 0.f), Math::vec3(1.f));

			AddGameObject(t_object);
			AddGameObject(t_object2);
			// DON'T ADD MORE THAN ONE LIGHT FOR ONE
			AddGameObject(t_light);

			//Load();
		}

		void Scene::Update(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, float a_deltatime)
		{
			//m_objs[0]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::quat(Math::vec3(0.01f * a_deltatime, 0.f,0.f)));
			m_transformSystem->Update();

			std::vector<std::pair<int, Math::UniformMatrixs>> t_syncro;
			std::vector<std::pair<int, Math::vec3>> t_lightSyncro;
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
					t_syncro.emplace_back(t_meshId, t_obj->GetComponent<TransformComponent>()->GetTransform()->GetUniformsMatrixs());
				}

				const int t_lightId = t_obj->GetComponentID<LightComponent>();
				if (std::cmp_not_equal(t_lightId, -1))
				{
					t_lightSyncro.emplace_back(t_lightId, t_obj->GetComponent<TransformComponent>()->GetTransform()->GetGlobalPosition());
				}
			}
			m_renderSystem->Update(a_renderer, t_syncro, t_lightSyncro);

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
			for (int i = 0; i < m_renderSystem->GetMeshComponents().size(); ++i)
			{
				t_vertexBuffersMap[GetDescriptorTarget(i)].push_back(m_renderSystem->GetMeshComponents().at(i)->GetMesh()->GetVertexBuffer());
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
				t_indexBuffersMap[GetDescriptorTarget(i)].push_back(m_renderSystem->GetMeshComponents().at(i)->GetMesh()->GetIndexBuffer());
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
				t_nbIndicesMap[GetDescriptorTarget(i)].push_back(m_renderSystem->GetMeshComponents().at(i)->GetMesh()->GetNbIndices());
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
			for(auto & t_descriptor : t_descriptors)
			{
				t_descriptorsMap[t_descriptor->GetPipelineTargetType()].push_back(t_descriptor);
			}
			return t_descriptorsMap;
		}

		std::vector<Core::RHI::IObjectDescriptor*> Scene::GetLightsDescriptors()
		{
			return m_renderSystem->GetLightDescriptors();
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
			LOG_ERROR("Tired to get descriptor at an invalid index!");
			return Core::RHI::DescriptorSetPipelineTarget::LitDescriptor;
		}


		void Scene::Destroy(Core::Renderer* a_renderer)
		{
			Save();

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

		void Scene::Save()
		{
			json t_scene;

			for (const auto& t_obj : m_objs)
			{
				json t_objJson;
				t_objJson["GameObject"] = t_obj->GetName();
				t_objJson["TransformComponent"] = SerializeTransformComponent(*t_obj->GetComponent<TransformComponent>());
				if (const RigidBodyComponent* t_rigidBodyComponent = t_obj->GetComponent<RigidBodyComponent>())
				{
					t_objJson["RigidBodyComponent"] = SerializeRigidBodyComponent(*t_rigidBodyComponent);
				}
				t_scene.push_back(t_objJson);
			}

			std::ofstream t_file(m_name + ".json");
			if (t_file.is_open())
			{
				t_file << t_scene.dump(4);
				t_file.close();
			}
		}

		void Scene::Load()
		{
			std::ifstream t_file(m_name + ".json");
			if (!t_file.is_open())
			{
				return;
			}

			json t_scene;
			t_file >> t_scene;

			for (const auto& t_entry : t_scene) {
				bool t_hasRigidBody = false;

				std::string t_name = t_entry.at("GameObject").get<std::string>();

				TransformData t_transform = DeserializeTransformComponent(t_entry.at("TransformComponent"));

				RigidBodyData t_rigidBody{};
				if (t_entry.contains("RigidBodyComponent")) 
				{
					t_rigidBody = DeserializeRigidBodyComponent(t_entry.at("RigidBodyComponent"));
					t_hasRigidBody = true;
				}

				LOG_DEBUG(t_name);

				LOG_DEBUG("Transform :");
				LOG_DEBUG("pos :");
				t_transform.mPos.Print();
				LOG_DEBUG("rot :");
				t_transform.mRot.Print();
				LOG_DEBUG("scale :");
				t_transform.mScale.Print();
				LOG_DEBUG("parent : " + Core::Debugging::ToString(t_transform.mParentId));

				if (t_hasRigidBody)
				{
					LOG_DEBUG("RigidBody :");
					LOG_DEBUG("body type :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mBodyType));
					LOG_DEBUG("layer :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mLayer));
					LOG_DEBUG("collider type :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mColliderType));
					LOG_DEBUG("pos :");
					t_rigidBody.mPos.Print();
					LOG_DEBUG("scale :");
					t_rigidBody.mScale.Print();
					LOG_DEBUG("radius :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mRadius));
					LOG_DEBUG("half height :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mHalfHeight));
					LOG_DEBUG("rot :");
					t_rigidBody.mRot.Print();
					LOG_DEBUG("mass :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mMass));
					LOG_DEBUG("enable :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mEnable));
					LOG_DEBUG("lock rotation X :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mLockRotX));
					LOG_DEBUG("lock rotation Y :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mLockRotY));
					LOG_DEBUG("lock rotation Z :");
					LOG_DEBUG(Core::Debugging::ToString(t_rigidBody.mLockRotZ));
				}
			}
		}

		nlohmann::ordered_json Scene::SerializeTransformComponent(const TransformComponent& a_transform)
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

			return t_outData;
		}
	}
}
