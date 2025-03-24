#ifndef SCENE_H
#define SCENE_H

#include "EngineExport.h"

#include "ComponentsPool.h"
#include "TransformSystem.h"
#include "RenderSystem.h"

namespace Engine
{
	namespace GamePlay
	{
		class Scene
		{
		public:


		private:
			ComponentsPool* m_componentsPool = nullptr;
			TransformSystem* m_transformSystem = nullptr;
			RenderSystem* m_renderSystem = nullptr;
		};
	}
}
#endif