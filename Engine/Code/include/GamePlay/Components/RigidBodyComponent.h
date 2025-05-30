#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include "GamePlay/Systems/EventSystem.h"
#include "Math/quat.h"
#include "Math/Transform.h"
#include "Math/vec3.h"

#include "Physics/RigidBody.h"
#include "Physics/Layers.h"
#include "Physics/ContactListener.h"

namespace Engine::GamePlay {
	class MeshComponent;
}

namespace Engine
{
	namespace GamePlay
	{
		struct RigidBodyData
		{
			int mBodyType;
			int mLayer;
			int mColliderType;
			Math::vec3 mPos;
			Math::vec3 mScale;
			float mRadius;
			float mHalfHeight;
			Math::quat mRot;
			float mMass;
			bool mEnable;
			bool mLockRotX;
			bool mLockRotY;
			bool mLockRotZ;
			int mMeshId;
			float mFriction;
			float mRestitution;
			MeshComponent* mMeshComponent;
			bool mIsTrigger;
		};

		class RigidBodyComponent : public Component
		{
		public:
			RigidBodyComponent() = default;
			~RigidBodyComponent() override = default;

			ENGINE_API static void Register();
			ENGINE_API void Create(int& a_outId) override;

			ENGINE_API void CreateBoxRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale, Math::quat a_rotation, const Math::Transform& a_transform, float a_mass = 1.f, bool a_enable = true);
			ENGINE_API void CreateSphereRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, float a_mass = 1.f, bool a_enable = true);
			ENGINE_API void CreateCapsuleRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, float a_mass = 1.f, bool a_enable = true);

			ENGINE_API void UpdateFromTransform(const Math::Transform* a_transform, bool a_enable = true);
			ENGINE_API void UpdateObjectTransform(Math::Transform* a_transform, float a_alpha);

			ENGINE_API void NotifyCollision(JPH::CollisionEvent a_collisionEvent, RigidBodyComponent* a_otherRigidBodyComponent) const;

			ENGINE_API void SetLinearVelocity(Math::vec3 a_velocity) const;
			ENGINE_API void AddForce(Math::vec3 a_force) const;
			ENGINE_API void AddImpulse(Math::vec3 a_impulse) const;

			ENGINE_API void SetFromData(const RigidBodyData& a_data) {}

			ENGINE_API void SetColliderType(Physics::ColliderType a_type);

			ENGINE_API void Destroy();

			ENGINE_API void SetPosition(const Math::vec3& a_pos, bool a_enable = true);
			ENGINE_API void Rotate(const Math::quat& a_rot, bool a_enable = true);
			ENGINE_API void SetRotation(const Math::quat& a_rot, bool a_enable = true);
			ENGINE_API void SetDebug(const bool a_debug) { m_debug = a_debug; }

			ENGINE_API void SetActive(const bool a_enable) const { m_rigidBody->SetActive(a_enable); }
			ENGINE_API void SetIsTrigger(const bool a_isTrigger) const { m_rigidBody->SetIsTrigger(a_isTrigger); }
			ENGINE_API void SetMass(const float a_mass) const { m_rigidBody->SetMass(a_mass); }

			ENGINE_API void LockRotation(const char a_axis) const;
			ENGINE_API void UnlockRotation(const char a_axis) const;

			ENGINE_API static void RemoveComponent(int a_id);
			ENGINE_API void RemoveMesh();

			ENGINE_API [[nodiscard]] bool IsTrigger() const { return m_rigidBody->IsTrigger(); }
			ENGINE_API [[nodiscard]] Physics::ColliderType GetColliderType() const { return m_rigidBody->GetColliderType(); }
			ENGINE_API [[nodiscard]] Physics::RigidBody* GetBody() const { return m_rigidBody; }
			ENGINE_API [[nodiscard]] Math::vec3 GetPos() const { return m_localPos; }
			ENGINE_API [[nodiscard]] Math::vec3 GetGlobalPos() const { return m_bodyPos; }
			ENGINE_API [[nodiscard]] Math::quat GetRot() const { return m_localRot; }
			ENGINE_API [[nodiscard]] Math::quat GetGlobalRot() const { return m_bodyRot; }
			ENGINE_API [[nodiscard]] bool GetDebug() const { return m_debug; }
			ENGINE_API [[nodiscard]] float GetMass() const { return m_rigidBody->GetMass(); }
			ENGINE_API [[nodiscard]] float GetFriction() const { return m_rigidBody->GetFriction(); }
			ENGINE_API [[nodiscard]] float GetRestitution() const { return m_rigidBody->GetRestitution(); }
			ENGINE_API [[nodiscard]] RigidBodyData GetRigidBodyData() const;
			ENGINE_API [[nodiscard]] MeshComponent* GetMeshComponent() const { return m_meshComponent; }
			ENGINE_API [[nodiscard]] int GetMeshID() const { return m_meshId; }
			ENGINE_API [[nodiscard]] Math::vec3 GetVelocity();

			ENGINE_API [[nodiscard]] bool IsGrounded(Math::vec3 a_pos, Math::quat a_rot);

			ENGINE_API static RigidBodyComponent* GetComponent(uint32_t a_id);

			ENGINE_API [[nodiscard]] CoolerEvent<RigidBodyComponent*>& GetOnCollisionEnterEvent() { return m_onCollisionEnterEvent; }
			ENGINE_API [[nodiscard]] CoolerEvent<RigidBodyComponent*>& GetOnCollisionStayEvent() { return m_onCollisionStayEvent; }
			ENGINE_API [[nodiscard]] CoolerEvent<RigidBodyComponent*>& GetOnCollisionExitEvent() { return m_onCollisionExitEvent; }
			ENGINE_API [[nodiscard]] CoolerEvent<RigidBodyComponent*>& GetOnTriggerEnterEvent() { return m_onTriggerEnterEvent; }
			ENGINE_API [[nodiscard]] CoolerEvent<RigidBodyComponent*>& GetOnTriggerStayEvent() { return m_onTriggerStayEvent; }
			ENGINE_API [[nodiscard]] CoolerEvent<RigidBodyComponent*>& GetOnTriggerExitEvent() { return m_onTriggerExitEvent; }
		private:
			ENGINE_API void OnCollisionEnter(RigidBodyComponent* a_otherRigidBodyComponent) const;
			ENGINE_API void OnCollisionStay(RigidBodyComponent* a_otherRigidBodyComponent) const;
			ENGINE_API void OnCollisionExit(RigidBodyComponent* a_otherRigidBodyComponent) const;
			ENGINE_API void OnTriggerEnter(RigidBodyComponent* a_otherRigidBodyComponent) const;
			ENGINE_API void OnTriggerStay(RigidBodyComponent* a_otherRigidBodyComponent) const;
			ENGINE_API void OnTriggerExit(RigidBodyComponent* a_otherRigidBodyComponent) const;

			Physics::RigidBody* m_rigidBody = nullptr;
			Math::vec3 m_localPos;
			Math::quat m_localRot;
			Math::vec3 m_bodyPos;
			Math::quat m_bodyRot;
			bool m_debug = false;
			MeshComponent* m_meshComponent = nullptr;
			int m_meshId = -1;

			CoolerEvent<RigidBodyComponent*> m_onCollisionEnterEvent;
			CoolerEvent<RigidBodyComponent*> m_onCollisionStayEvent;
			CoolerEvent<RigidBodyComponent*> m_onCollisionExitEvent;
			CoolerEvent<RigidBodyComponent*> m_onTriggerEnterEvent;
			CoolerEvent<RigidBodyComponent*> m_onTriggerStayEvent;
			CoolerEvent<RigidBodyComponent*> m_onTriggerExitEvent;
		};
	}
}

#endif