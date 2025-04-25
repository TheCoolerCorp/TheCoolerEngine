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

			ENGINE_API ComponentType Create(int& a_outId, bool a_colliderMesh = false);

			ENGINE_API void Destroy();

			ENGINE_API void Set(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale);
			ENGINE_API void Set(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

			ENGINE_API void Move(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale);
			ENGINE_API void Move(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

			ENGINE_API void SetGameObject(int a_gameObjectId) { m_gameObjectId = a_gameObjectId; }
			ENGINE_API void SetParent(int a_id);
			ENGINE_API void RemoveParent();
			ENGINE_API void AddChild(int a_id);
			ENGINE_API void RemoveChild(int a_id);
			ENGINE_API void ClearChildren();

			[[nodiscard]] ENGINE_API Math::Transform* GetTransform() const { return m_transform; }
			[[nodiscard]] ENGINE_API int GetID() const { return m_id; }
			[[nodiscard]] ENGINE_API int GetGameObjectID() const { return m_gameObjectId; }
			[[nodiscard]] ENGINE_API int GetParentID() const { return m_parentId; }
			[[nodiscard]] ENGINE_API std::vector<int> GetChildrenIDs() const { return m_childrenIds; }

			ENGINE_API static TransformComponent* GetComponent(int a_id);
			ENGINE_API static void RemoveComponent(int a_id);

			ENGINE_API static ComponentType GetType(bool a_colliderMesh = false) { return ComponentType::TRANSFORM; }

		private:
			Math::Transform* m_transform = nullptr;

			int m_id = -1;
			int m_gameObjectId = -1;
			int m_parentId = -1;
			std::vector<int> m_childrenIds{};
		};
	}
}

#endif 