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
			ENGINE_API void Update() const;
			ENGINE_API void Destroy();

			ENGINE_API uint32_t AddTransformComponent(TransformComponent* a_component);
			ENGINE_API void RemoveTransformComponent(uint32_t a_id);

			ENGINE_API uint32_t GetSize() { return m_components.size(); }
		private:
			std::vector<TransformComponent*> m_components{};
			std::vector<uint32_t> m_availableIds{};

		};
	}
}
#endif