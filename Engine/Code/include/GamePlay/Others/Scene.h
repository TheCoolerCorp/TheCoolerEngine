#ifndef SCENE_H
#define SCENE_H

#include "EngineExport.h"

#include "Gameplay/Systems/TransformSystem.h"
#include "Gameplay/Systems/RenderSystem.h"
#include "Core/Renderer/Renderer.h"
#include "Ressources/ResourceManager.h"
#include "GamePlay/Others/GameObject.h"
#include <nlohmann/json.hpp>
#include "GamePlay/Others/Camera.h"

namespace Engine
{
	namespace GamePlay
	{
		enum GameObjectType
		{
			OBJECTTYPE_EMPTY,
			OBJECTTYPE_CUBE,
			OBJECTTYPE_SPHERE,
			OBJECTTYPE_PLANE,
			OBJECTTYPE_LIGHT,
			OBJECTTYPE_CAMERA
		};

		class Scene
		{
		public:
			~Scene() = default;

			ENGINE_API void Create(Core::Renderer* a_renderer, const char* a_name, int a_width, int a_height);
			ENGINE_API void Update(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, float a_deltatime);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> GetVertexBuffers();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> GetIndexBuffers();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>> GetNBIndices();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IObjectDescriptor*>> GetDescriptors();
			[[nodiscard]] ENGINE_API std::vector<Core::RHI::IObjectDescriptor*> GetLightsDescriptors();
			[[nodiscard]] ENGINE_API Core::RHI::DescriptorSetPipelineTarget GetDescriptorTarget(int a_idx);

			ENGINE_API Core::RHI::IObjectDescriptor* GetCameraDescriptor() { return m_mainCamera->GetDescriptor(); }
			ENGINE_API void AddGameObject(GameObject* a_object, uint32_t a_parentTransformId = -1, std::vector<uint32_t> a_childTransformIds = {});
			ENGINE_API GameObject* AddGameObject(GameObjectType a_type, uint32_t a_parentTransformId = -1, std::vector<uint32_t> a_childTransformIds = {});
			ENGINE_API void RemoveGameObject(uint32_t a_id);
			ENGINE_API [[nodiscard]] GameObject* GetGameObject(const uint32_t a_id) const { return m_objs[a_id]; }
			ENGINE_API [[nodiscard]] int GetObjectCount() const { return static_cast<int>(m_objs.size()); }
			ENGINE_API [[nodiscard]] std::string& GetName() { return m_name; }
			ENGINE_API [[nodiscard]] RenderSystem* GetRenderSystem() { return m_renderSystem; }
			ENGINE_API void Save();
			ENGINE_API void Load(Core::Renderer* a_renderer);
			ENGINE_API void SetMode(bool a_mode) { m_isPlaying = a_mode; }
			ENGINE_API bool IsPlaying() { return m_isPlaying; }
		private:
			static nlohmann::ordered_json SerializeTransformComponent(const TransformComponent& a_transform);
			static TransformData DeserializeTransformComponent(const nlohmann::ordered_json& a_json);
			static nlohmann::ordered_json SerializeRigidBodyComponent(const RigidBodyComponent& a_rigidBody);
			static RigidBodyData DeserializeRigidBodyComponent(const nlohmann::ordered_json& a_json);
			static nlohmann::ordered_json SerializeMeshComponent(const MeshComponent& a_mesh);
			static MeshData DeserializeMeshComponent(const nlohmann::ordered_json& a_json);

			std::vector<GameObject*> m_objs;
			std::vector<int> m_availableIds;
			Camera* m_mainCamera;
			Core::Renderer* m_renderer = nullptr;

			std::string m_name{};
			TransformSystem* m_transformSystem = nullptr;
			RenderSystem* m_renderSystem = nullptr;
			PhysicsSystem* m_physicsSystem = nullptr;

			bool m_isPlaying = false;
		};
	}	
}
#endif