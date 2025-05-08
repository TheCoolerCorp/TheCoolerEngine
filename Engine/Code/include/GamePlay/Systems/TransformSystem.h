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
			ENGINE_API ~TransformSystem() override = default;

			ENGINE_API void Create();
			ENGINE_API void Update();
			ENGINE_API void UpdateChild(int a_id, bool a_force = false);
			ENGINE_API void Destroy();

			ENGINE_API int AddComponent(TransformComponent* a_component);
			ENGINE_API TransformComponent* GetComponent(int a_id);
			ENGINE_API void RemoveComponent(int a_id);
			ENGINE_API bool IsValidId(int id);

			ENGINE_API int GetSize() { return static_cast<int>(m_components.size()); }
		private:
			Math::mat4 GetParentsMatrix(int a_id);

			std::vector<TransformComponent*> m_components{};
			std::vector<int> m_availableIds{};
		};
	}
}
#endif