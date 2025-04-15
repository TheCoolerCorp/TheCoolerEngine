#include "GamePlay/Components/RigidBodyComponent.h"

#include "Gameplay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType RigidBodyComponent::Create(uint32_t& a_outId)
		{
			a_outId = ServiceLocator::GetPhysicsSystem()->AddComponent(this);
			return ComponentType::RIGIDBODY;
		}

		void RigidBodyComponent::CreateBoxRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale,
		                                            Math::quat a_rotation, const bool a_enable)
		{
			m_rigidBody.CreateBoxBody(a_type, a_layer, a_position, a_scale, a_rotation, a_enable);
		}

		void RigidBodyComponent::CreateSphereRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_radius, Math::quat a_rotation, bool a_enable)
		{
			m_rigidBody.CreateSphereBody(a_type, a_layer, a_position, a_radius, a_rotation, a_enable);
		}

		void RigidBodyComponent::Destroy()
		{
			
		}
	}
}
