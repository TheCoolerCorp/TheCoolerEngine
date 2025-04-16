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

		void RigidBodyComponent::CreateCapsuleRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, bool a_enable)
		{
			m_rigidBody.CreateCapsuleBody(a_type, a_layer, a_position, a_halfHeight, a_radius, a_rotation, a_enable);
		}

		void RigidBodyComponent::UpdateObjectTransform(Math::Transform* a_transform) const
		{
			const JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_bodyID = m_rigidBody.GetBodyID();
			const JPH::Vec3 t_bodyPos = t_bodyInterface->GetCenterOfMassPosition(t_bodyID);
			const JPH::Quat t_bodyRot = t_bodyInterface->GetRotation(t_bodyID);

			const Math::vec3 t_newPos = { t_bodyPos.GetX(), t_bodyPos.GetY(), t_bodyPos.GetZ() };
			const Math::quat t_newRot = { t_bodyRot.GetX(), t_bodyRot.GetY(), t_bodyRot.GetZ(), t_bodyRot.GetW() };

			const Math::mat4 t_newTRS = Math::mat4::TRS(t_newPos, t_newRot, a_transform->GetGlobalScale());
			a_transform->SetMatrix(t_newTRS);
		}

		void RigidBodyComponent::Destroy()
		{
			m_rigidBody.Remove();
			m_rigidBody.Destroy();
		}

		RigidBodyComponent* RigidBodyComponent::GetComponent(const uint32_t a_id)
		{
			return ServiceLocator::GetPhysicsSystem()->GetComponent(a_id);
		}
	}
}
