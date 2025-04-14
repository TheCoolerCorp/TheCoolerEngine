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

namespace Engine
{
	namespace Physics
	{
		class RigidBody
		{
		public:

			ENGINE_API [[nodiscard]] JPH::BodyID GetBodyID() const;

		private:
			JPH::Body* m_body = nullptr;
		};
	}
}

#endif