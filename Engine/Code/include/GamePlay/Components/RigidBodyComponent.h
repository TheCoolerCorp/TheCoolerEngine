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

			ENGINE_API ComponentType Create(uint32_t& a_outId);

			ENGINE_API void CreateBoxRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale, Math::quat a_rotation, bool a_enable = true);
			ENGINE_API void CreateSphereRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, float a_radius, Math::quat a_rotation, bool a_enable = true);
			ENGINE_API void CreateCapsuleRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, bool a_enable = true);

			ENGINE_API void UpdateObjectTransform(Math::Transform* a_transform) const;

			ENGINE_API void Destroy();

			ENGINE_API static ComponentType GetType() { return ComponentType::RIGIDBODY; }

		private:
			Physics::RigidBody m_rigidBody;
		};
	}
}

#endif