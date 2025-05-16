#ifndef SCENE_H
#define SCENE_H

#include "EngineExport.h"

#include "Gameplay/Systems/TransformSystem.h"
#include "Gameplay/Systems/RenderSystem.h"
#include "Core/Renderer/Renderer.h"
#include "Ressources/ResourceManager.h"
#include "GamePlay/Others/GameObject.h"
#include <nlohmann/json.hpp>

#include "GamePlay/Components/PlayerControllerComponent.h"
#include "GamePlay/ComponentsBase/Camera.h"
#include "GamePlay/Systems/EventSystem.h"
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

			ENGINE_API void Create(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, const char* a_name, int a_width, int a_height);
			ENGINE_API void Update(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, float a_deltatime);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> GetVertexBuffers();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> GetIndexBuffers();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>> GetNBIndices();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IObjectDescriptor*>> GetDescriptors();
			[[nodiscard]] ENGINE_API Core::RHI::IObjectDescriptor* GetLightsDescriptors();
			[[nodiscard]] ENGINE_API Core::RHI::DescriptorSetPipelineTarget GetDescriptorTarget(int a_idx);

			ENGINE_API Core::RHI::IObjectDescriptor* GetCameraDescriptor();
			ENGINE_API void AddGameObject(GameObject* a_object, uint32_t a_parentTransformId = -1, std::vector<uint32_t> a_childTransformIds = {});
			ENGINE_API GameObject* AddGameObject(GameObjectType a_type, uint32_t a_parentTransformId = -1, std::vector<uint32_t> a_childTransformIds = {});
			ENGINE_API void RemoveGameObject(uint32_t a_id);
			ENGINE_API [[nodiscard]] GameObject* GetGameObject(const uint32_t a_id) const { return m_objs[a_id]; }
			ENGINE_API [[nodiscard]] int GetObjectCount() const { return static_cast<int>(m_objs.size()); }
			ENGINE_API [[nodiscard]] std::string& GetName() { return m_name; }
			ENGINE_API [[nodiscard]] RenderSystem* GetRenderSystem() { return m_renderSystem; }
			ENGINE_API [[nodiscard]] Camera* GetMainCamera() const { return m_mainCamera; }
			ENGINE_API [[nodiscard]] CoolerEvent<>& GetBeginPlayEvent() { return m_beginPlayEvent; }
			ENGINE_API [[nodiscard]] CoolerEvent<>& GetEndPlayEvent() { return m_endPlayEvent; }
			ENGINE_API [[nodiscard]] static bool ProcessKeyboardInputs() { return m_processKeyboardInputs; }

			ENGINE_API void Save();
			ENGINE_API void Load(Core::Renderer* a_renderer);
			ENGINE_API void Reload(Core::Renderer* a_renderer);
			ENGINE_API bool SetMode(bool a_mode);
			[[nodiscard]] ENGINE_API bool IsPlaying() const { return m_isPlaying; }
			ENGINE_API bool IsPlaying() { return m_isPlaying; }
			ENGINE_API bool HasObject(int a_id);
			ENGINE_API static void SetProcessKeyboardInputs(bool a_process) { m_processKeyboardInputs = a_process; }

		private:
			static nlohmann::ordered_json SerializeTransformComponent(const TransformComponent& a_transform, std::vector<std::pair<int, int>> a_oldAndNewIndexes);
			static TransformData DeserializeTransformComponent(const nlohmann::ordered_json& a_json);
			static nlohmann::ordered_json SerializeRigidBodyComponent(const RigidBodyComponent& a_rigidBody);
			static RigidBodyData DeserializeRigidBodyComponent(const nlohmann::ordered_json& a_json);
			static nlohmann::ordered_json SerializeMeshComponent(const MeshComponent& a_mesh);
			static MeshData DeserializeMeshComponent(const nlohmann::ordered_json& a_json);
			static nlohmann::ordered_json SerializeLightComponent(const LightComponent& a_lightComponent);
			static LightData DeserializeLightComponent(const nlohmann::ordered_json& a_json);
			static nlohmann::ordered_json SerializeCameraComponent(const CameraComponent& a_cameraComponent);
			static CameraSerializeData DeserializeCameraComponent(const nlohmann::ordered_json& a_json);
			static nlohmann::ordered_json SerializePlayerControllerComponent(const PlayerControllerComponent& a_gameObject);
			static PlayerControllerData DeserializePlayerControllerComponent(const nlohmann::ordered_json& a_json);

			void SetMainCamera(const int a_objectId) { m_mainCameraObjectId = a_objectId; }

			CoolerEvent<> m_beginPlayEvent = CoolerEvent<>();
			CoolerEvent<> m_endPlayEvent = CoolerEvent<>();

			std::vector<GameObject*> m_objs;
			std::vector<int> m_deletionQueue;
			std::vector<int> m_availableIds;
			Camera* m_mainCamera;
			Core::Renderer* m_renderer = nullptr;

			std::string m_name{};
			TransformSystem* m_transformSystem = nullptr;
			RenderSystem* m_renderSystem = nullptr;
			PhysicsSystem* m_physicsSystem = nullptr;
			GameComponentSystem* m_gameComponentSystem = nullptr;

			bool m_isPlaying = false;
			bool m_lastState = false;
			bool m_justReloaded = false;
			static bool m_processKeyboardInputs;
			int m_gameCameraId = 0;
			int m_mainCameraObjectId = 0;

			int m_width = 0;
			int m_height = 0;
		};
	}	
}
#endif