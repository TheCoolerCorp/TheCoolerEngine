#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "EngineExport.h"
#include "Math/quat.h"

namespace Engine
{
	namespace Physics
	{
		enum class BodyType
		{
			STATIC = 0,
			KINEMATIC,
			DYNAMIC
		};

		enum class CollisionLayer
		{
			NON_MOVING = 0,
			MOVING,
			TRIGGER,
			DISABLED
		};

		enum class ColliderType
		{
			BOX = 0,
			SPHERE,
			CAPSULE
		};

		class RigidBody
		{
		public:
			ENGINE_API void CreateBoxBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale, Math::quat a_rotation, bool a_enable);
			ENGINE_API void CreateSphereBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, float a_radius, Math::quat a_rotation, bool a_enable);
			ENGINE_API void CreateCapsuleBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, bool a_enable);

			ENGINE_API void SetActive(bool a_enable);
			ENGINE_API void SetIsTrigger(bool a_trigger);
			ENGINE_API [[nodiscard]] JPH::BodyID GetBodyID() const;
			ENGINE_API [[nodiscard]] bool IsActive() const { return m_isActive; }
			ENGINE_API [[nodiscard]] ColliderType GetColliderType() const { return m_colliderType; }
			ENGINE_API [[nodiscard]] BodyType GetBodyType() const { return m_bodyType; }
			ENGINE_API [[nodiscard]] CollisionLayer GetLayer() const { return m_collisionLayer; }
			ENGINE_API [[nodiscard]] Math::vec3 GetScale() const { return m_scale; }
			ENGINE_API [[nodiscard]] float GetRadius() const { return m_radius; }
			ENGINE_API [[nodiscard]] float GetHalfHeight() const { return m_halfHeight; }
			ENGINE_API void Remove() const;
			ENGINE_API void Destroy();

		private:
			JPH::Body* m_body = nullptr;

			Math::vec3 m_scale = Math::vec3();
			float m_radius = 0.f, m_halfHeight = 0.f;

			ColliderType m_colliderType = ColliderType::BOX;
			BodyType m_bodyType = BodyType::STATIC;
			CollisionLayer m_collisionLayer = CollisionLayer::DISABLED;
			bool m_isActive = false;

			static JPH::EMotionType BodyTypeToJPHType(BodyType a_type);
			static JPH::ObjectLayer CollisionLayerToJPHLayer(CollisionLayer a_layer);
		};
	}
}

#endif