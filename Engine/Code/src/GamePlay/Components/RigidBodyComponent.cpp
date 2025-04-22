#include "GamePlay/Components/RigidBodyComponent.h"

#include "Gameplay/ServiceLocator.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType RigidBodyComponent::Create(uint32_t& a_outId, bool a_colliderMesh)
		{
			a_outId = ServiceLocator::GetPhysicsSystem()->AddComponent(this);
			return ComponentType::RIGIDBODY;
		}

		void RigidBodyComponent::CreateBoxRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale,
		                                            Math::quat a_rotation, const Math::Transform& a_transform, const bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetPosition();
			const Math::quat t_rot = a_transform.GetRotation() * m_localRot;
			m_oldGlobalPos = t_pos;
			m_oldGlobalRot = t_rot;

			m_rigidBody.CreateBoxBody(a_type, a_layer, t_pos, a_scale, t_rot, a_enable);
		}

		void RigidBodyComponent::CreateSphereRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetPosition();
			const Math::quat t_rot = a_transform.GetRotation() * m_localRot;
			m_oldGlobalPos = t_pos;
			m_oldGlobalRot = t_rot;

			m_rigidBody.CreateSphereBody(a_type, a_layer, t_pos, a_radius, t_rot, a_enable);
		}

		void RigidBodyComponent::CreateCapsuleRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetPosition();
			const Math::quat t_rot = a_transform.GetRotation() * m_localRot;
			m_oldGlobalPos = t_pos;
			m_oldGlobalRot = t_rot;

			m_rigidBody.CreateCapsuleBody(a_type, a_layer, t_pos, a_halfHeight, a_radius, t_rot, a_enable);
		}

		void RigidBodyComponent::UpdateObjectTransform(Math::Transform* a_transform)
		{
			/*
			const JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_bodyID = m_rigidBody.GetBodyID();
			const JPH::Vec3 t_bodyPos = t_bodyInterface->GetCenterOfMassPosition(t_bodyID);
			const JPH::Quat t_bodyRot = t_bodyInterface->GetRotation(t_bodyID);

			Math::vec3 t_newPos = { t_bodyPos.GetX(), t_bodyPos.GetY(), t_bodyPos.GetZ() };
			Math::vec3 t_deltaPos = t_newPos - m_oldGlobalPos;
			Math::quat t_newRot = Math::quat::Normalize({ t_bodyRot.GetX(), t_bodyRot.GetY(), t_bodyRot.GetZ(), t_bodyRot.GetW() });
			Math::quat t_deltaRot = t_newRot * Math::quat::Conjugate(m_oldGlobalRot);

			Math::vec3 oldOffsetPos = m_oldGlobalRot * m_localPos;
			Math::vec3 newOffsetPos = t_deltaRot * m_localPos;

			Math::vec3 offsetDelta = newOffsetPos - oldOffsetPos;
			Math::vec3 fullDeltaPos = t_deltaPos + offsetDelta;

			a_transform->Translate(fullDeltaPos);
			m_oldGlobalPos = t_newPos;
			a_transform->Rotate(t_deltaRot);
			m_oldGlobalRot = t_newRot;
			m_bodyRot = t_newRot;
			*/

			/*
			const JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_bodyID = m_rigidBody.GetBodyID();
			const JPH::Vec3 t_bodyPos = t_bodyInterface->GetCenterOfMassPosition(t_bodyID);
			const JPH::Quat t_bodyRot = t_bodyInterface->GetRotation(t_bodyID);

			Math::vec3 t_newPos = { t_bodyPos.GetX(), t_bodyPos.GetY(), t_bodyPos.GetZ() };
			Math::vec3 t_deltaPos = t_newPos - m_oldGlobalPos;
			Math::quat t_newRot = Math::quat::Normalize({ t_bodyRot.GetX(), t_bodyRot.GetY(), t_bodyRot.GetZ(), t_bodyRot.GetW() });
			Math::quat t_deltaRot = t_newRot * Math::quat::Conjugate(m_oldGlobalRot);

			Math::vec3 pivotCorrection = m_localPos - (Math::quat::Normalize(t_deltaRot) * m_localPos);
			pivotCorrection.Print();

			a_transform->Translate(t_deltaPos + pivotCorrection);
			m_oldGlobalPos = t_newPos;
			a_transform->Rotate(t_deltaRot);
			m_oldGlobalRot = t_newRot;
			m_bodyRot = t_newRot;
			*/

			const JPH::BodyInterface* bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID bodyID = m_rigidBody.GetBodyID();

			// Get world-space rigid body transform
			const JPH::Vec3 bodyPos = bodyInterface->GetCenterOfMassPosition(bodyID);
			const JPH::Quat bodyRot = bodyInterface->GetRotation(bodyID);

			// Convert to Math types
			Math::vec3 worldPos = { bodyPos.GetX(), bodyPos.GetY(), bodyPos.GetZ() };
			Math::quat worldRot = Math::quat::Normalize({ bodyRot.GetX(), bodyRot.GetY(), bodyRot.GetZ(), bodyRot.GetW() });

			// Apply local offset to world transform
			Math::vec3 finalPos = worldPos - (worldRot * m_localPos);
			Math::quat finalRot = worldRot * Math::quat::Conjugate(m_localRot);

			// Set transform directly
			a_transform->SetPosition(finalPos);
			a_transform->SetRotation(Math::quat::Normalize(finalRot));

			// Update cached state
			m_oldGlobalPos = worldPos;
			m_oldGlobalRot = worldRot;
			m_bodyRot = worldRot;
		}

		void RigidBodyComponent::Destroy()
		{
			m_rigidBody.Remove();
			m_rigidBody.Destroy();
		}

		void RigidBodyComponent::SetPosition(const Math::vec3& a_pos, const bool a_enable) const
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			t_bodyInterface->SetPosition(m_rigidBody.GetBodyID(), { a_pos.x, a_pos.y, a_pos.z }, t_activation);
		}

		void RigidBodyComponent::Rotate(const Math::quat& a_rot, const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			m_localRot *= a_rot;

			const JPH::BodyID t_bodyID = m_rigidBody.GetBodyID();
			const JPH::Quat t_currentRot = t_bodyInterface->GetRotation(t_bodyID);

			JPH::Quat t_newRot = t_currentRot * JPH::Quat(a_rot.x, a_rot.y, a_rot.z, a_rot.w);

			t_newRot = t_newRot.Normalized();

			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
			t_bodyInterface->SetRotation(t_bodyID, t_newRot, t_activation);

			m_bodyRot = Math::quat(t_newRot.GetX(), t_newRot.GetY(), t_newRot.GetZ(), t_newRot.GetW());
		}

		RigidBodyComponent* RigidBodyComponent::GetComponent(const uint32_t a_id)
		{
			return ServiceLocator::GetPhysicsSystem()->GetComponent(a_id);
		}
	}
}
