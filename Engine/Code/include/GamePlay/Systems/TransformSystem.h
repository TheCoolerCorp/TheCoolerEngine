#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include "EngineExport.h"

#include "System.h"
#include "GamePlay/Components/TransformComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		class TransformSystem : public System
		{
		public:
			ENGINE_API TransformSystem() = default;
			ENGINE_API ~TransformSystem() = default;

			ENGINE_API void Create();
			ENGINE_API void Update();
			ENGINE_API void Destroy();

		private:
			std::vector<TransformComponent*> m_component{};
			std::vector<uint32_t> m_availableIds{};

		};
	}
}
#endif