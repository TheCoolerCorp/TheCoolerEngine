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

			ENGINE_API void UpdateFromTransform(const Math::Transform* a_transform, bool a_enable = true);
			ENGINE_API void UpdateObjectTransform(Math::Transform* a_transform);

			ENGINE_API void NotifyCollision(JPH::CollisionEvent a_collisionEvent) const;

			ENGINE_API void SetOnCollisionEnter(std::function<void()> a_event);
			ENGINE_API void SetOnCollisionStay(std::function<void()> a_event);
			ENGINE_API void SetOnCollisionExit(std::function<void()> a_event);
			ENGINE_API void SetOnTriggerEnter(std::function<void()> a_event);
			ENGINE_API void SetOnTriggerStay(std::function<void()> a_event);
			ENGINE_API void SetOnTriggerExit(std::function<void()> a_event);

			ENGINE_API void SetLinearVelocity(Math::vec3 a_velocity) const;

			ENGINE_API void Destroy();

			ENGINE_API void SetPosition(const Math::vec3& a_pos, bool a_enable = true) const;
			ENGINE_API void Rotate(const Math::quat& a_rot, bool a_enable = true);
			ENGINE_API void SetDebug(const bool a_debug) { m_debug = a_debug; }

			ENGINE_API void SetActive(const bool a_enable) const { m_rigidBody->SetActive(a_enable); }
			ENGINE_API void SetIsTrigger(const bool a_isTrigger) const { m_rigidBody->SetIsTrigger(a_isTrigger); }
			
			ENGINE_API [[nodiscard]] bool IsTrigger() const { return m_rigidBody->IsTrigger(); }
			ENGINE_API [[nodiscard]] Physics::ColliderType GetColliderType() const { return m_rigidBody->GetColliderType(); }
			ENGINE_API [[nodiscard]] Physics::RigidBody* GetBody() const { return m_rigidBody; }
			ENGINE_API [[nodiscard]] Math::vec3 GetPos() const { return m_bodyPos; }
			ENGINE_API [[nodiscard]] Math::quat GetRot() const { return m_bodyRot; }
			ENGINE_API [[nodiscard]] bool GetDebug() const { return m_debug; }

			ENGINE_API static ComponentType GetType(bool a_colliderMesh = false) { return ComponentType::RIGIDBODY; }
			ENGINE_API static RigidBodyComponent* GetComponent(uint32_t a_id);

		private:
			ENGINE_API void OnCollisionEnter() const;
			ENGINE_API void OnCollisionStay() const;
			ENGINE_API void OnCollisionExit() const;
			ENGINE_API void OnTriggerEnter() const;
			ENGINE_API void OnTriggerStay() const;
			ENGINE_API void OnTriggerExit() const;

			Physics::RigidBody* m_rigidBody = nullptr;
			Math::vec3 m_localPos;
			Math::quat m_localRot;
			Math::vec3 m_bodyPos;
			Math::quat m_bodyRot;
			bool m_debug = false;
			std::function<void()> m_onCollisionEnter;
			std::function<void()> m_onCollisionStay;
			std::function<void()> m_onCollisionExit;
			std::function<void()> m_onTriggerEnter;
			std::function<void()> m_onTriggerStay;
			std::function<void()> m_onTriggerExit;
		};
	}
}

#endif