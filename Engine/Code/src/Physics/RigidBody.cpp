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
			m_scale = a_scale;
			m_colliderType = ColliderType::BOX;
			m_bodyType = a_type;
			m_collisionLayer = a_layer;
			m_isActive = a_enable;

			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			const JPH::Vec3 t_halfExtent = { a_scale.x * 0.5f, a_scale.y * 0.5f, a_scale.z * 0.5f };
			const JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			const JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };

			const JPH::EMotionType t_motionType = BodyTypeToJPHType(a_type);
			const JPH::ObjectLayer t_layer = CollisionLayerToJPHLayer(a_layer);
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			JPH::BodyCreationSettings t_bodySettings(new JPH::BoxShape(t_halfExtent), t_position, t_rotation, t_motionType, t_layer);
			t_bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
			t_bodySettings.mMassPropertiesOverride.mMass = 1.f;
			m_mass = 1.f;
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::CreateSphereBody(const BodyType a_type, const CollisionLayer a_layer, const Math::vec3 a_position, const float a_radius,
			const Math::quat a_rotation, const bool a_enable)
		{
			m_radius = a_radius;
			m_colliderType = ColliderType::SPHERE;
			m_bodyType = a_type;
			m_collisionLayer = a_layer;
			m_isActive = a_enable;

			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			const JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			const JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };

			const JPH::EMotionType t_motionType = BodyTypeToJPHType(a_type);
			const JPH::ObjectLayer t_layer = CollisionLayerToJPHLayer(a_layer);
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			JPH::BodyCreationSettings t_bodySettings(new JPH::SphereShape(a_radius), t_position, t_rotation, t_motionType, t_layer);
			t_bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
			t_bodySettings.mMassPropertiesOverride.mMass = 1.f;
			m_mass = 1.f;
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::CreateCapsuleBody(const BodyType a_type, const CollisionLayer a_layer, const Math::vec3 a_position,
			const float a_halfHeight, const float a_radius, const Math::quat a_rotation, const bool a_enable)
		{
			m_halfHeight = a_halfHeight;
			m_radius = a_radius;
			m_colliderType = ColliderType::CAPSULE;
			m_bodyType = a_type;
			m_collisionLayer = a_layer;
			m_isActive = a_enable;

			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			const JPH::Vec3 t_position = { a_position.x, a_position.y, a_position.z };
			const JPH::Quat t_rotation = { a_rotation.x, a_rotation.y, a_rotation.z, a_rotation.w };

			const JPH::EMotionType t_motionType = BodyTypeToJPHType(a_type);
			const JPH::ObjectLayer t_layer = CollisionLayerToJPHLayer(a_layer);
			const JPH::EActivation t_activation = a_enable ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;

			JPH::BodyCreationSettings t_bodySettings(new JPH::CapsuleShape(m_halfHeight, m_radius), t_position, t_rotation, t_motionType, t_layer);
			t_bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
			t_bodySettings.mMassPropertiesOverride.mMass = 1.f;
			m_mass = 1.f;
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::SetActive(const bool a_enable)
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_id = m_body->GetID();
			if (a_enable && !this->m_isActive)
			{
				this->m_isActive = true;
				t_bodyInterface->SetObjectLayer(t_id, CollisionLayerToJPHLayer(m_collisionLayer));
				t_bodyInterface->SetMotionType(t_id, BodyTypeToJPHType(m_bodyType), JPH::EActivation::Activate);
			}
			else if (this->m_isActive)
			{
				this->m_isActive = false;
				t_bodyInterface->SetObjectLayer(t_id, CollisionLayerToJPHLayer(CollisionLayer::DISABLED));
				t_bodyInterface->SetMotionType(t_id, BodyTypeToJPHType(BodyType::STATIC), JPH::EActivation::DontActivate);
			}
		}

		void RigidBody::SetIsTrigger(const bool a_trigger)
		{
			m_collisionLayer = CollisionLayer::TRIGGER;
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_id = m_body->GetID();
			t_bodyInterface->SetObjectLayer(t_id, CollisionLayerToJPHLayer(m_collisionLayer));
			m_body->SetIsSensor(a_trigger);
		}

		void RigidBody::SetMass(const float a_mass)
		{
			if (!m_isActive || m_bodyType != BodyType::DYNAMIC)
			{
				return;
			}

			m_mass = a_mass;

			JPH::MotionProperties* t_motionProperties = m_body->GetMotionProperties();
			const JPH::Shape* t_shape = m_body->GetShape();
			JPH::MassProperties t_massProperties = t_shape->GetMassProperties();

			t_massProperties.ScaleToMass(m_mass);
			t_massProperties.mMass = m_mass;
			t_motionProperties->SetMassProperties(JPH::EAllowedDOFs::All, t_massProperties);
		}

		JPH::BodyID RigidBody::GetBodyID() const
		{
			return m_body->GetID();
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
			case CollisionLayer::TRIGGER:
				return JPH::Layers::TRIGGER;
			case CollisionLayer::DISABLED:
				return JPH::Layers::DISABLED;
			}
			return JPH::Layers::MOVING;
		}
	}
}
