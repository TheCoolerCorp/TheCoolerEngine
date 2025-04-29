#ifndef SCENE_H
#define SCENE_H

#include "EngineExport.h"

#include "Gameplay/Systems/TransformSystem.h"
#include "Gameplay/Systems/MeshRendererSystem.h"
#include "Core/Renderer/Renderer.h"
#include "Ressources/ResourceManager.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Others/Camera.h"

namespace Engine
{
	namespace GamePlay
	{
		class Scene
		{
		public:
			~Scene() = default;

			ENGINE_API void Create(Core::Renderer* a_renderer, int a_width, int a_height);
			ENGINE_API void Update(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, float a_deltatime);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> GetVertexBuffers();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IBuffer*>> GetIndexBuffers();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>> GetNBIndices();
			[[nodiscard]] ENGINE_API std::unordered_map<Core::RHI::DescriptorSetPipelineTarget, std::vector<Core::RHI::IObjectDescriptor*>> GetDescriptors();
			[[nodiscard]] ENGINE_API Core::RHI::DescriptorSetPipelineTarget GetDescriptorTarget(int a_idx);

			ENGINE_API Core::RHI::IObjectDescriptor* GetCameraDescriptor() { return m_mainCamera->GetDescriptor(); }
			ENGINE_API void AddGameObject(GameObject* a_object, uint32_t a_parentTransformId = -1, std::vector<uint32_t> a_childTransformIds = {});
			ENGINE_API void RemoveGameObject(uint32_t a_id);
			ENGINE_API GameObject* GetGameObject(uint32_t a_id) const { return m_objs[a_id]; }
			ENGINE_API int GetObjectCount() { return static_cast<int>(m_objs.size()); }
		private:
			std::vector<GameObject*> m_objs;
			std::vector<int> m_availableIds;
			Camera* m_mainCamera;
			TransformSystem* m_transformSystem = nullptr;
			MeshRendererSystem* m_meshRendererSystem = nullptr;
			PhysicsSystem* m_physicsSystem = nullptr;
			Resource::ResourceManager* m_resourceManager = nullptr;
		};
	}	
}
#endif