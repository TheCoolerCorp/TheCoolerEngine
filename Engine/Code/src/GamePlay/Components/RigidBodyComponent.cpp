#include <utility>

#include "GamePlay/Components/RigidBodyComponent.h"

#include "Gameplay/ServiceLocator.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType RigidBodyComponent::Create(uint32_t& a_outId, bool a_colliderMesh)
		{
			m_rigidBody = new Physics::RigidBody;
			a_outId = ServiceLocator::GetPhysicsSystem()->AddComponent(this);
			return ComponentType::RIGIDBODY;
		}

		void RigidBodyComponent::CreateBoxRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer, Math::vec3 a_position, Math::vec3 a_scale,
		                                            Math::quat a_rotation, const Math::Transform& a_transform, const bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetGlobalPosition();
			const Math::quat t_rot = a_transform.GetGlobalRotation() * m_localRot;
			m_bodyPos = t_pos;
			m_bodyRot = t_rot;

			m_rigidBody->CreateBoxBody(a_type, a_layer, t_pos, a_scale, t_rot, a_enable);
		}

		void RigidBodyComponent::CreateSphereRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetGlobalPosition();
			const Math::quat t_rot = a_transform.GetGlobalRotation() * m_localRot;
			m_bodyPos = t_pos;
			m_bodyRot = t_rot;

			m_rigidBody->CreateSphereBody(a_type, a_layer, t_pos, a_radius, t_rot, a_enable);
		}

		void RigidBodyComponent::CreateCapsuleRigidBody(Physics::BodyType a_type, Physics::CollisionLayer a_layer,
			Math::vec3 a_position, float a_halfHeight, float a_radius, Math::quat a_rotation, const Math::Transform& a_transform, bool a_enable)
		{
			m_localPos = a_position;
			m_localRot = a_rotation;
			const Math::vec3 t_pos = m_localPos + a_transform.GetGlobalPosition();
			const Math::quat t_rot = a_transform.GetGlobalRotation() * m_localRot;
			m_bodyPos = t_pos;
			m_bodyRot = t_rot;

			m_rigidBody->CreateCapsuleBody(a_type, a_layer, t_pos, a_halfHeight, a_radius, t_rot, a_enable);
		}

		void RigidBodyComponent::UpdateFromTransform(const Math::Transform* a_transform, const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
			const JPH::BodyID t_bodyID = m_rigidBody->GetBodyID();
			const Math::vec3 t_worldPos = a_transform->GetGlobalPosition() + (a_transform->GetGlobalRotation() * m_localPos);
			const Math::quat t_worldRot = Math::quat::Normalize(a_transform->GetGlobalRotation() * m_localRot);

			const JPH::Vec3 t_jphPos(t_worldPos.x, t_worldPos.y, t_worldPos.z);
			const JPH::Quat t_jphRot(t_worldRot.x, t_worldRot.y, t_worldRot.z, t_worldRot.w);

			t_bodyInterface->SetPosition(t_bodyID, t_jphPos, t_activation);
			m_bodyPos = t_worldPos;
			t_bodyInterface->SetRotation(t_bodyID, t_jphRot, t_activation);
			m_bodyRot = t_worldRot;
		}

		void RigidBodyComponent::UpdateObjectTransform(Math::Transform* a_transform)
		{
			const JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_bodyId = m_rigidBody->GetBodyID();
			const JPH::Vec3 t_bodyPos = t_bodyInterface->GetCenterOfMassPosition(t_bodyId);
			const JPH::Quat t_bodyRot = t_bodyInterface->GetRotation(t_bodyId);

			const Math::vec3 t_worldPos = { t_bodyPos.GetX(), t_bodyPos.GetY(), t_bodyPos.GetZ() };
			const Math::quat t_worldRot = Math::quat::Normalize({ t_bodyRot.GetX(), t_bodyRot.GetY(), t_bodyRot.GetZ(), t_bodyRot.GetW() });
			const Math::vec3 t_finalPos = t_worldPos - (t_worldRot * m_localPos);
			const Math::quat t_finalRot = t_worldRot * Math::quat::Conjugate(m_localRot);

			a_transform->Translate(t_finalPos - a_transform->GetGlobalPosition());
			m_bodyPos = t_worldPos;
			a_transform->Rotate(Math::quat::Normalize(t_finalRot) * Math::quat::Normalize(Math::quat::Conjugate(a_transform->GetGlobalRotation())));
			m_bodyRot = t_worldRot;
		}

		void RigidBodyComponent::OnCollisionEnter() const
		{
			if (!m_onCollisionEnter)
			{
				return;
			}

			m_onCollisionEnter();
		}

		void RigidBodyComponent::OnCollisionStay() const
		{
			if (!m_onCollisionStay)
			{
				return;
			}

			m_onCollisionStay();
		}

		void RigidBodyComponent::OnCollisionExit() const
		{
			if (!m_onCollisionExit)
			{
				return;
			}

			m_onCollisionExit();
		}

		void RigidBodyComponent::OnTriggerEnter() const
		{
			if (!m_onTriggerEnter)
			{
				return;
			}

			m_onTriggerEnter();
		}

		void RigidBodyComponent::OnTriggerStay() const
		{
			if (!m_onTriggerStay)
			{
				return;
			}

			m_onTriggerStay();
		}

		void RigidBodyComponent::OnTriggerExit() const
		{
			if (!m_onTriggerExit)
			{
				return;
			}

			m_onTriggerExit();
		}

		void RigidBodyComponent::SetOnCollisionEnter(std::function<void()> a_event)
		{
			m_onCollisionEnter = std::move(a_event);
		}

		void RigidBodyComponent::SetOnCollisionStay(std::function<void()> a_event)
		{
			m_onCollisionStay = std::move(a_event);
		}

		void RigidBodyComponent::SetOnCollisionExit(std::function<void()> a_event)
		{
			m_onCollisionExit = std::move(a_event);
		}

		void RigidBodyComponent::SetOnTriggerEnter(std::function<void()> a_event)
		{
			m_onTriggerEnter = std::move(a_event);
		}

		void RigidBodyComponent::SetOnTriggerStay(std::function<void()> a_event)
		{
			m_onTriggerStay = std::move(a_event);
		}

		void RigidBodyComponent::SetOnTriggerExit(std::function<void()> a_event)
		{
			m_onTriggerExit = std::move(a_event);
		}

		void RigidBodyComponent::Destroy()
		{
			m_rigidBody->Remove();
			m_rigidBody->Destroy();
			delete m_rigidBody;
			m_rigidBody = nullptr;
		}

		void RigidBodyComponent::SetPosition(const Math::vec3& a_pos, const bool a_enable) const
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			t_bodyInterface->SetPosition(m_rigidBody->GetBodyID(), { a_pos.x, a_pos.y, a_pos.z }, t_activation);
		}

		void RigidBodyComponent::Rotate(const Math::quat& a_rot, const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			m_localRot *= a_rot;

			const JPH::BodyID t_bodyID = m_rigidBody->GetBodyID();
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
