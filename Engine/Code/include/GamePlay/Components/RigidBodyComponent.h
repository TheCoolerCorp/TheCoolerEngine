#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include "Math/quat.h"
#include "Math/Transform.h"
#include "Math/vec3.h"

#include "Physics/RigidBody.h"
#include "Physics/Layers.h"
#include "Physics/ContactListener.h"

namespace Engine
{
	namespace GamePlay
	{
		class RigidBodyComponent : public Component
		{
		public:
			RigidBodyComponent() = default;
			~RigidBodyComponent() override = default;

			ENGINE_API ComponentType Create(uint32_t& a_outId, bool a_colliderMesh = false);

			ENGINE_API void CreateBoxRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale, Math::quat a_rotation, const Math::Transform& a_transform, bool a_enable = true);
			ENGINE_API void CreateSphereRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, bool a_enable = true);
			ENGINE_API void CreateCapsuleRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, bool a_enable = true);

			ENGINE_API void UpdateObjectTransform(Math::Transform* a_transform);

			ENGINE_API void Destroy();

			ENGINE_API void SetPosition(const Math::vec3& a_pos, bool a_enable = true) const;
			ENGINE_API void Rotate(const Math::quat& a_rot, bool a_enable = true);
			ENGINE_API void SetDebug(bool t_debug) { m_debug = t_debug; }

			ENGINE_API [[nodiscard]] Physics::ColliderType GetBodyType() const { return m_rigidBody.GetType(); }
			ENGINE_API [[nodiscard]] Physics::RigidBody GetBody() const { return m_rigidBody; }
			ENGINE_API [[nodiscard]] Math::vec3 GetPos() const { return m_localPos; }
			ENGINE_API [[nodiscard]] Math::quat GetRot() const { return m_bodyRot; }
			ENGINE_API [[nodiscard]] bool GetDebug() const { return m_debug; }

			ENGINE_API static ComponentType GetType(bool a_colliderMesh = false) { return ComponentType::RIGIDBODY; }
			ENGINE_API static RigidBodyComponent* GetComponent(uint32_t a_id);

		private:
			Physics::RigidBody m_rigidBody;
			Math::vec3 m_localPos;
			Math::vec3 m_oldGlobalPos;
			Math::quat m_localRot;
			Math::quat m_bodyRot;
			Math::quat m_oldGlobalRot;
			bool m_debug = false;
		};
	}
}

#endif