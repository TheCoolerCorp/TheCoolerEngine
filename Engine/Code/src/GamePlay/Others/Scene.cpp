#include "GamePlay/Others/Scene.h"

#include <utility>
#include <meta/factory.hpp>
#include <meta/meta.hpp>

#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Others/GameObject.h"
#include "Math/TheCoolerMath.h"

using json = nlohmann::ordered_json;

namespace Engine
{
	namespace GamePlay
	{
		void Scene::Create(Core::Renderer* a_renderer, const char* a_name)
		{
			m_name = a_name;

			m_transformSystem = new TransformSystem;

			m_meshRendererSystem = new MeshRendererSystem;

			m_physicsSystem = new PhysicsSystem;
			m_physicsSystem->Create();

			m_resourceManager = new Resource::ResourceManager;

			ServiceLocator::ProvideResourceManager(m_resourceManager);
			ServiceLocator::ProvideTransformSystem(m_transformSystem);
			ServiceLocator::ProvideRendererSystem(m_meshRendererSystem);
			ServiceLocator::ProvidePhysicsSystem(m_physicsSystem);

			Ref<Resource::Mesh> t_capsuleCollider = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/WireframeCapsule.obj");
			t_capsuleCollider->Load(a_renderer);
			Ref<Resource::Mesh> t_cubeCollider = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/WireframeCube.obj");
			t_cubeCollider->Load(a_renderer);
			Ref<Resource::Mesh> t_sphereCollider = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/WireframeSphere.obj");
			t_sphereCollider->Load(a_renderer);
			Ref<Resource::Texture> t_colliderTexture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/ColliderTexture.png");
			t_colliderTexture->Load(a_renderer);

			Ref<Resource::Mesh> t_mesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/viking_room.obj");
			Ref<Resource::Texture> t_texture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh->Load(a_renderer);
			t_texture->Load(a_renderer);

			GameObject* t_object = new GameObject();
			t_object->GetComponent<TransformComponent>()->Set({ Math::vec3(0.f), Math::quat(Math::vec3(Math::ToRadians(0.f), 0.f, 0.f)), Math::vec3(1.f), -1 });
			t_object->AddComponent<MeshComponent>();
			t_object->AddComponent<RigidBodyComponent>();
			RigidBodyComponent* t_rigidBodyComponent = t_object->GetComponent<RigidBodyComponent>();
			if (t_rigidBodyComponent)
			{
				t_rigidBodyComponent->CreateBoxRigidBody(Physics::BodyType::STATIC, Physics::CollisionLayer::NON_MOVING, Math::vec3(0.f, 0.f, 0.f), Math::vec3(2.f), Math::quat(Math::vec3(Math::ToRadians(0.f), 0.f, 0.f)), *t_object->GetComponent<TransformComponent>()->GetTransform());
				t_rigidBodyComponent->SetDebug(true);
				t_rigidBodyComponent->SetActive(false);
				t_rigidBodyComponent->SetActive(true);
			}
			t_object->AddComponent<MeshComponent>(true);
			MeshComponent* t_debugMeshComponent = t_object->GetComponent<MeshComponent>(true);
			t_object->GetComponent<MeshComponent>(true)->SetMesh(t_cubeCollider);
			t_object->GetComponent<MeshComponent>(true)->SetTexture(t_colliderTexture);

			t_object->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object->GetComponent<MeshComponent>()->SetTexture(t_texture);

			AddGameObject(t_object);

			Ref<Resource::Mesh> t_mesh2 = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/FinalBaseMesh.obj");
			Ref<Resource::Texture> t_texture2 = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/viking_room.png");
			t_mesh2->Load(a_renderer);
			t_texture2->Load(a_renderer);

			GameObject* t_object2 = new GameObject();
			t_object2->GetComponent<TransformComponent>()->Set({ Math::vec3(0.f, 3.f, 0.f), Math::quat(Math::vec3(0.f, 0.f, 0.f)), Math::vec3(0.2f), -1 });
			t_object2->AddComponent<MeshComponent>();
			t_object2->AddComponent<RigidBodyComponent>();
			RigidBodyComponent* t_rigidBodyComponent2 = t_object2->GetComponent<RigidBodyComponent>();
			if (t_rigidBodyComponent2)
			{
				t_rigidBodyComponent2->CreateCapsuleRigidBody(Physics::BodyType::DYNAMIC, Physics::CollisionLayer::MOVING, Math::vec3(0.f, 2.f, 0.f), 1.f, 1.f, Math::quat(Math::vec3(Math::ToRadians(0.f), 0.f, 0.f)), *t_object2->GetComponent<TransformComponent>()->GetTransform());
				t_rigidBodyComponent2->SetDebug(true);
				t_rigidBodyComponent2->SetOnCollisionExit([this](RigidBodyComponent* a_rigidBodyComponent) { TestFunc(a_rigidBodyComponent); });
				t_rigidBodyComponent2->LockRotation('x');
				t_rigidBodyComponent2->LockRotation('z');
				//t_rigidBodyComponent2->UnlockRotation('x');
			}
			t_object2->AddComponent<MeshComponent>(true);
			t_object2->GetComponent<MeshComponent>(true)->SetMesh(t_capsuleCollider);
			t_object2->GetComponent<MeshComponent>(true)->SetTexture(t_colliderTexture);

			t_object2->GetComponent<MeshComponent>()->SetMesh(t_mesh2);
			t_object2->GetComponent<MeshComponent>()->SetTexture(t_texture2);

			AddGameObject(t_object2);

			/*GameObject* t_object3 = new GameObject(Math::vec3(0.f), Math::vec3(0.f, Math::ToRadians(270.f), 0.f), Math::vec3(1.f));
			t_object3->AddComponent<MeshComponent>();


			t_object3->GetComponent<MeshComponent>()->SetMesh(t_mesh);
			t_object3->GetComponent<MeshComponent>()->SetTexture(t_texture);*/

			//m_objs.push_back(t_object3);

			Load();
		}

		void Scene::Update(Core::Renderer* a_renderer, const float a_deltaTime)
		{
			m_objs[0]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::quat(Math::vec3(0.01f * a_deltaTime, 0.f,0.f)));
			//m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetScale(Math::vec3(0.05f));
			//m_objs[1]->GetComponent<TransformComponent>()->GetTransform()->SetPosition(Math::vec3(5.f, 0.f, 0.f));
			//m_objs[1]->GetComponent<TransformComponent>()->SetParent(m_objs[0]->GetComponentID<TransformComponent>());
			//m_objs[2]->GetComponent<TransformComponent>()->SetParent(m_objs[1]->GetComponentID<TransformComponent>());
			//m_objs[2]->GetComponent<TransformComponent>()->GetTransform()->Rotate(Math::vec3(0.f, 0.05f, 0.f));
			m_transformSystem->Update();

			std::vector<std::pair<int, Math::mat4>> t_syncro;
			std::vector<Math::Transform*> t_physicsTransforms;

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
			m_physicsSystem->Update(a_deltaTime, t_physicsTransforms);

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

		void Scene::Draw(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Camera* a_camera)
		{
			m_meshRendererSystem->Render(a_renderer, a_window, a_camera);
		}

		void Scene::Destroy(Core::Renderer* a_renderer)
		{
			Save();

			for (int i = 0; i < m_objs.size(); ++i)
			{
				delete m_objs[i];
			}
			m_objs.clear();

			m_meshRendererSystem->Destroy(a_renderer);
			delete m_meshRendererSystem;

			m_transformSystem->Destroy();
			delete m_transformSystem;

			m_physicsSystem->Destroy();
			delete m_physicsSystem;
			m_physicsSystem = nullptr;

			m_resourceManager->DestroyAll(a_renderer);
			delete m_resourceManager;
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
				std::string t_name = t_entry.at("GameObject").get<std::string>();
				TransformData t_transform = (DeserializeTransformComponent(t_entry.at("TransformComponent")));

				LOG_DEBUG(t_name);
				LOG_DEBUG("Transform :");
				LOG_DEBUG("pos :");
				t_transform.mPos.Print();
				LOG_DEBUG("rot :");
				t_transform.mRot.Print();
				LOG_DEBUG("scale :");
				t_transform.mScale.Print();
				LOG_DEBUG("parent : " + Core::Debugging::ToString(t_transform.mParentId));
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

		void Scene::TestFunc(RigidBodyComponent* a_rigidBodyComponent)
		{
			m_objs[1]->GetComponent<RigidBodyComponent>()->AddForce({ 0.f, 10000.f, 0.f });
		}
	}
}
