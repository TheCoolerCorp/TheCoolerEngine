#ifndef SCENE_H
#define SCENE_H

#include "EngineExport.h"

#include "ComponentsPool.h"
#include "TransformSystem.h"
#include "RenderSystem.h"
#include "Core/Renderer/Renderer.h"

namespace Engine
{
	namespace GamePlay
	{
		class Scene
		{
		public:
			~Scene() = default;

			ENGINE_API void Update();
			ENGINE_API void Render(Core::Renderer* a_renderer);
		private:
			ComponentsPool* m_componentsPool = nullptr;
			TransformSystem* m_transformSystem = nullptr;
			RenderSystem* m_renderSystem = nullptr;
		};
	}
}
#endif