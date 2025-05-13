#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "EngineExport.h"
#include "Jolt/Physics/Constraints/SixDOFConstraint.h"
#include "Math/quat.h"

namespace Engine
{
	namespace Physics
	{
		enum class BodyType : uint8_t
		{
			STATIC = 0,
			KINEMATIC,
			DYNAMIC
		};

		enum class CollisionLayer : uint8_t
		{
			NON_MOVING = 0,
			MOVING,
			TRIGGER,
			DISABLED
		};

		enum class ColliderType : uint8_t
		{
			BOX = 0,
			SPHERE,
			CAPSULE
		};

		class RigidBody
		{
		public:
			ENGINE_API void CreateBoxBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale, Math::quat a_rotation, float a_mass, bool a_enable);
			ENGINE_API void CreateSphereBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, float a_radius, Math::quat a_rotation, float a_mass, bool a_enable);
			ENGINE_API void CreateCapsuleBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, float a_mass, bool a_enable);

			ENGINE_API void SetActive(bool a_enable);
			ENGINE_API void SetScale(Math::vec3 a_scale);
			ENGINE_API void SetRadius(float a_radius);
			ENGINE_API void SetHalfHeight(float a_halfHeight);
			ENGINE_API void SetIsTrigger(bool a_trigger);
			ENGINE_API void SetMass(float a_mass);
			ENGINE_API void LockRotation(const char a_axis);
			ENGINE_API void UnlockRotation(const char a_axis);
			ENGINE_API void SetFriction(float a_friction);
			ENGINE_API void SetRestitution(float a_restitution);

			ENGINE_API void SetColliderType(ColliderType a_type);
			ENGINE_API void SetObjectLayer(CollisionLayer a_layer);
			ENGINE_API void SetBodyType(BodyType a_type);

			ENGINE_API [[nodiscard]] bool IsTrigger() const { return m_body->IsSensor(); }
			ENGINE_API [[nodiscard]] bool IsRotLockedX() const;
			ENGINE_API [[nodiscard]] bool IsRotLockedY() const;
			ENGINE_API [[nodiscard]] bool IsRotLockedZ() const;
			ENGINE_API [[nodiscard]] JPH::BodyID GetBodyID() const;
			ENGINE_API [[nodiscard]] JPH::Body* GetBody() const { return m_body; }
			ENGINE_API [[nodiscard]] bool IsActive() const { return m_isActive; }
			ENGINE_API [[nodiscard]] ColliderType GetColliderType() const { return m_colliderType; }
			ENGINE_API [[nodiscard]] BodyType GetBodyType() const { return m_bodyType; }
			ENGINE_API [[nodiscard]] CollisionLayer GetLayer() const { return m_collisionLayer; }
			ENGINE_API [[nodiscard]] Math::vec3 GetScale() const { return m_scale; }
			ENGINE_API [[nodiscard]] float GetRadius() const { return m_radius; }
			ENGINE_API [[nodiscard]] float GetHalfHeight() const { return m_halfHeight; }
			ENGINE_API [[nodiscard]] float GetMass() const { return m_mass; }
			ENGINE_API [[nodiscard]] float GetFriction() const { return m_friction; }
			ENGINE_API [[nodiscard]] float GetRestitution() const { return m_restitution; }
			ENGINE_API void Remove() const;
			ENGINE_API void Destroy();

		private:
			JPH::Body* m_body = nullptr;

			Math::vec3 m_scale = Math::vec3();
			float m_radius = 1.f, m_halfHeight = 1.f;

			ColliderType m_colliderType = ColliderType::BOX;
			BodyType m_bodyType = BodyType::STATIC;
			CollisionLayer m_collisionLayer = CollisionLayer::DISABLED;
			bool m_isActive = false;
			float m_friction = 0.5f;
			float m_restitution = 0.0f;

			float m_mass = 1.f;

			std::unordered_map<char, JPH::SixDOFConstraint*> m_lockedRotationAxes{};

			static JPH::EMotionType BodyTypeToJPHType(BodyType a_type);
			static JPH::ObjectLayer CollisionLayerToJPHLayer(CollisionLayer a_layer);

			void DestroyBody();
		};
	}
}

#endif