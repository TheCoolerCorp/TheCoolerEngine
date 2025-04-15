#include "Physics/RigidBody.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "Gameplay/ServiceLocator.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"

namespace Engine
{
	namespace Physics
	{
		void RigidBody::CreateBoxBody(const BodyType a_type, const CollisionLayer a_layer, const Math::vec3 a_position,
			const Math::vec3 a_scale, const Math::quat a_rotation, const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			const JPH::Vec3 t_halfExtent = { a_scale.x * 0.5f, a_scale.y * 0.5f, a_scale.z * 0.5f };
			const JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			const JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };

			const JPH::EMotionType t_motionType = BodyTypeToJPHType(a_type);
			const JPH::ObjectLayer t_layer = CollisionLayerToJPHLayer(a_layer);
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			const JPH::BodyCreationSettings t_bodySettings(new JPH::BoxShape(t_halfExtent), t_position, t_rotation, t_motionType, t_layer);
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::CreateSphereBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position, float a_radius,
			Math::quat a_rotation, bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			const JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			const JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };

			const JPH::EMotionType t_motionType = BodyTypeToJPHType(a_type);
			const JPH::ObjectLayer t_layer = CollisionLayerToJPHLayer(a_layer);
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			const JPH::BodyCreationSettings t_bodySettings(new JPH::SphereShape(a_radius), t_position, t_rotation, t_motionType, t_layer);
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::CreateCapsuleBody(BodyType a_type, CollisionLayer a_layer, Math::vec3 a_position,
			float a_halfHeight, float a_radius, Math::quat a_rotation, bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			const JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			const JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };

			const JPH::EMotionType t_motionType = BodyTypeToJPHType(a_type);
			const JPH::ObjectLayer t_layer = CollisionLayerToJPHLayer(a_layer);
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			const JPH::BodyCreationSettings t_bodySettings(new JPH::CapsuleShape(a_halfHeight, a_radius), t_position, t_rotation, t_motionType, t_layer);
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

		void RigidBody::Remove() const
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			t_bodyInterface->RemoveBody(m_body->GetID());
		}

		void RigidBody::Destroy()
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			t_bodyInterface->DestroyBody(m_body->GetID());
			delete m_body;
			m_body = nullptr;
		}

		JPH::EMotionType RigidBody::BodyTypeToJPHType(const BodyType a_type)
		{
			switch (a_type)
			{
			case BodyType::STATIC:
				return JPH::EMotionType::Static;
			case BodyType::KINEMATIC:
				return JPH::EMotionType::Kinematic;
			case BodyType::DYNAMIC:
				return JPH::EMotionType::Dynamic;
			}
			return JPH::EMotionType::Dynamic;
		}

		JPH::ObjectLayer RigidBody::CollisionLayerToJPHLayer(const CollisionLayer a_layer)
		{
			switch (a_layer)
			{
			case CollisionLayer::MOVING:
				return JPH::Layers::MOVING;
			case CollisionLayer::NON_MOVING:
				return JPH::Layers::NON_MOVING;
			}
			return JPH::Layers::MOVING;
		}
	}
}
