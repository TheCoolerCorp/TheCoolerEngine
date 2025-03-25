#ifndef COMPONENTSPOOL_H
#define COMPONENTSPOOL_H

#include "EngineExport.h"

#include "GamePlay/TransformComponent.h"
#include "GamePlay/Meshcomponent.h"

#include <unordered_map>

namespace Engine
{
	namespace GamePlay
	{
		class ComponentsPool
		{
		public:
			~ComponentsPool() = default;

			ENGINE_API void Init();
			ENGINE_API void Destroy();

			

		private:
			std::unordered_map<int, TransformComponent*> m_transformComponents;
			std::unordered_map<int, MeshComponent*> m_meshesComponents;
			std::vector<int> m_registerId;
		};
	}
}
#endif