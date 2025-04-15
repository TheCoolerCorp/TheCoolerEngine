#include "Physics/RigidBody.h"

#include "Gameplay/ServiceLocator.h"

namespace Engine
{
	namespace Physics
	{
		void RigidBody::CreateBoxBody(const BodyType a_type, const CollisionLayer a_layer, const Math::vec3 a_position,
			const Math::vec3 a_scale, const Math::quat a_rotation, const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			JPH::Vec3 t_halfExtent = { a_scale.x * 0.5f, a_scale.y * 0.5f, a_scale.z * 0.5f };
			JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };
			JPH::EMotionType t_motionType;
			JPH::ObjectLayer t_layer;
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			switch (a_type)
			{
			case BodyType::STATIC:
				t_motionType = JPH::EMotionType::Static;
				break;
			case BodyType::KINEMATIC:
				t_motionType = JPH::EMotionType::Kinematic;
				break;
			case BodyType::DYNAMIC:
				t_motionType = JPH::EMotionType::Dynamic;
				break;
			}

			switch (a_layer)
			{
			case CollisionLayer::MOVING:
				t_layer = JPH::Layers::MOVING;
				break;
			case CollisionLayer::NON_MOVING:
				t_layer = JPH::Layers::NON_MOVING;
				break;
			}

			JPH::BodyCreationSettings t_bodySettings(new JPH::BoxShape(t_halfExtent), t_position, t_rotation, t_motionType, t_layer);
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::CreateSphereBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, float a_radius,
			Math::quat a_rotation, bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };
			JPH::EMotionType t_motionType;
			JPH::ObjectLayer t_layer;
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			switch (a_type)
			{
			case BodyType::STATIC:
				t_motionType = JPH::EMotionType::Static;
				break;
			case BodyType::KINEMATIC:
				t_motionType = JPH::EMotionType::Kinematic;
				break;
			case BodyType::DYNAMIC:
				t_motionType = JPH::EMotionType::Dynamic;
				break;
			}

			switch (a_layer)
			{
			case CollisionLayer::MOVING:
				t_layer = JPH::Layers::MOVING;
				break;
			case CollisionLayer::NON_MOVING:
				t_layer = JPH::Layers::NON_MOVING;
				break;
			}

			JPH::BodyCreationSettings t_bodySettings(new JPH::SphereShape(a_radius), t_position, t_rotation, t_motionType, t_layer);
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::SetActive(const bool a_enable) const
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			if (a_enable)
			{
				t_bodyInterface->ActivateBody(m_body->GetID());
				return;
			}
			t_bodyInterface->DeactivateBody(m_body->GetID());
		}

		JPH::BodyID RigidBody::GetBodyID() const
		{
			return m_body->GetID();
		}

		bool RigidBody::IsActive() const
		{
			const JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			return t_bodyInterface->IsActive(m_body->GetID());
		}
	}
}
