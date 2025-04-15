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

			ENGINE_API void Create(Core::Renderer* a_renderer);
			ENGINE_API void Update(Core::Renderer* a_renderer);
			ENGINE_API void Draw(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, Camera* a_camera);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

		private:
			std::vector<GamePlay::GameObject*> m_objs;
			Camera* m_mainCamera;

			TransformSystem* m_transformSystem = nullptr;
			MeshRendererSystem* m_meshRendererSystem = nullptr;

			Resource::ResourceManager* m_resourceManager = nullptr;
		};
	}
}
#endif