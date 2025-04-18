#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include  "Math/Transform.h"

namespace Engine
{
	namespace GamePlay
	{
		class TransformComponent : public Component
		{
		public:
			TransformComponent() = default;
			~TransformComponent() override = default;

			ENGINE_API ComponentType Create(uint32_t& a_outId, bool a_colliderMesh = false);

			ENGINE_API void Destroy();

			ENGINE_API void Set(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale);
			ENGINE_API void Set(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

			ENGINE_API void Move(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale);
			ENGINE_API void Move(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

			ENGINE_API void SetParent(uint32_t a_id);

			ENGINE_API Math::Transform* GetTransform() const { return m_transform; }

			ENGINE_API static TransformComponent* GetComponent(uint32_t a_id);

			ENGINE_API static void RemoveComponent(uint32_t a_id);
			
			ENGINE_API uint32_t GetParentID() const  { return m_parentId; }

			ENGINE_API static ComponentType GetType(bool a_colliderMesh = false) { return ComponentType::TRANSFORM; }

		private:
			Math::Transform* m_transform = nullptr;

			uint32_t m_parentId = -1;
		};
	}
}

#endif 