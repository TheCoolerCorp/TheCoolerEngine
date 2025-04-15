#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

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
			MOVING
		};

		class RigidBody
		{
		public:
			ENGINE_API void CreateBoxBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale, Math::quat a_rotation, bool a_enable);
			ENGINE_API void CreateSphereBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, float a_radius, Math::quat a_rotation, bool a_enable);

			ENGINE_API void SetActive(bool a_enable) const;
			ENGINE_API [[nodiscard]] JPH::BodyID GetBodyID() const;
			ENGINE_API [[nodiscard]] bool IsActive() const;

		private:
			JPH::Body* m_body = nullptr;
		};
	}
}

#endif