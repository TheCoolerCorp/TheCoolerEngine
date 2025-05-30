#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include  "Math/Transform.h"

namespace Engine
{
	namespace GamePlay
	{
		struct TransformData
		{
			Math::vec3 mPos;
			Math::quat mRot;
			Math::vec3 mScale;
			int mParentId;
		};

		class TransformComponent : public Component
		{
		public:
			ENGINE_API TransformComponent() = default;
			ENGINE_API ~TransformComponent() override = default;

			ENGINE_API static void Register();
			ENGINE_API void Create(int& a_outId) override;

			ENGINE_API void Destroy() override;

			ENGINE_API void Set(const TransformData& a_data);
			ENGINE_API void SetPosition(Math::vec3 a_pos);
			ENGINE_API void SetRotation(Math::vec3 a_euler);
			ENGINE_API void SetRotation(Math::quat a_quat);
			ENGINE_API void SetScale(Math::vec3 a_scale);

			ENGINE_API [[nodiscard]] Math::vec3 GetGlobalPos();
			ENGINE_API [[nodiscard]] Math::vec3 GetParentGlobalPos();
			ENGINE_API [[nodiscard]] Math::vec3 GetGlobalEuler();
			ENGINE_API [[nodiscard]] Math::vec3 GetParentGlobalEuler();
			ENGINE_API [[nodiscard]] Math::quat GetGlobalRotation();
			ENGINE_API [[nodiscard]] Math::quat GetParentGlobalRotation();
			ENGINE_API [[nodiscard]] Math::vec3 GetGlobalScale();
			ENGINE_API [[nodiscard]] Math::vec3 GetParentGlobalScale();

			ENGINE_API void Move(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

			
			ENGINE_API void SetParent(int a_id);
			ENGINE_API void RemoveParent();
			ENGINE_API void AddChild(int a_id);
			ENGINE_API void RemoveChild(int a_id);
			ENGINE_API void ClearChildren();

			[[nodiscard]] ENGINE_API Math::Transform* GetTransform() const { return m_transform; }
			[[nodiscard]] ENGINE_API TransformData GetTransformData() const;
			[[nodiscard]] ENGINE_API int GetParentID() const { return m_parentId; }
			[[nodiscard]] ENGINE_API std::vector<int> GetChildrenIDs() const { return m_childrenIds; }
			[[nodiscard]] ENGINE_API Math::mat4 GetMatrix() { return m_transform->GetTransformMatrix(); }

			[[nodiscard]] ENGINE_API Math::vec3 GetForward() { return m_transform->GetForward(); }
			[[nodiscard]] ENGINE_API Math::vec3 GetRight() { return m_transform->GetRight(); }
			[[nodiscard]] ENGINE_API Math::vec3 GetUp() { return m_transform->GetUp(); }

			ENGINE_API static TransformComponent* GetComponent(int a_id);
			ENGINE_API static void RemoveComponent(int a_id);
		private:
			Math::Transform* m_transform = nullptr;

			int m_parentId = -1;
			std::vector<int> m_childrenIds{};
		};
	}
}

#endif 