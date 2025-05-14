#include "Physics/RigidBody.h"

#include <ranges>
#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "Gameplay/ServiceLocator.h"
#include "Jolt/Physics/Collision/CollisionCollectorImpl.h"
#include "Jolt/Physics/Collision/ShapeCast.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"

namespace Engine
{
	namespace Physics
	{
		void RigidBody::CreateBoxBody(const BodyType a_type, const CollisionLayer a_layer, const Math::vec3 a_position,
			const Math::vec3 a_scale, const Math::quat a_rotation, float a_mass, const bool a_enable)
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
			SetActive(a_enable);

			m_shape = new JPH::BoxShape(t_halfExtent);
			JPH::BodyCreationSettings t_bodySettings(m_shape, t_position, t_rotation, t_motionType, t_layer);
			t_bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
			t_bodySettings.mMassPropertiesOverride.mMass = a_mass;
			t_bodySettings.mAllowDynamicOrKinematic = true;
			m_mass = a_mass;
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::CreateSphereBody(const BodyType a_type, const CollisionLayer a_layer, const Math::vec3 a_position, const float a_radius,
			const Math::quat a_rotation, float a_mass, const bool a_enable)
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
			SetActive(a_enable);

			m_shape = new JPH::SphereShape(a_radius);
			JPH::BodyCreationSettings t_bodySettings(m_shape, t_position, t_rotation, t_motionType, t_layer);
			t_bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
			t_bodySettings.mMassPropertiesOverride.mMass = a_mass;
			t_bodySettings.mAllowDynamicOrKinematic = true;
			m_mass = a_mass;
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::CreateCapsuleBody(const BodyType a_type, const CollisionLayer a_layer, const Math::vec3 a_position,
			const float a_halfHeight, const float a_radius, const Math::quat a_rotation, float a_mass, const bool a_enable)
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
			SetActive(a_enable);

			m_shape = new JPH::CapsuleShape(m_halfHeight, m_radius);
			JPH::BodyCreationSettings t_bodySettings(m_shape, t_position, t_rotation, t_motionType, t_layer);
			t_bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
			t_bodySettings.mMassPropertiesOverride.mMass = a_mass;
			t_bodySettings.mAllowDynamicOrKinematic = true;
			m_mass = a_mass;
			m_body = t_bodyInterface->CreateBody(t_bodySettings);
			t_bodyInterface->AddBody(m_body->GetID(), t_activation);
		}

		void RigidBody::SetActive(const bool a_enable)
		{
			if (a_enable == m_isActive)
				return;

			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			const JPH::BodyID t_id = m_body->GetID();

			m_isActive = a_enable;

			if (a_enable)
			{
				t_bodyInterface->SetObjectLayer(t_id, CollisionLayerToJPHLayer(m_collisionLayer));
				t_bodyInterface->SetMotionType(t_id, BodyTypeToJPHType(m_bodyType), JPH::EActivation::Activate);
			}
			else
			{
				t_bodyInterface->SetObjectLayer(t_id, CollisionLayerToJPHLayer(CollisionLayer::DISABLED));
				t_bodyInterface->SetMotionType(t_id, BodyTypeToJPHType(BodyType::STATIC), JPH::EActivation::DontActivate);
			}
		}

		void RigidBody::SetScale(Math::vec3 a_scale)
		{
			if (m_colliderType != ColliderType::BOX)
			{
				LOG_ERROR("RigidBody::SetScale : Cannot set scale on non-box collider!");
				return;
			}
			if (a_scale.x < 0.f || a_scale.y < 0.f || a_scale.z < 0.f)
			{
				LOG_ERROR("RigidBody::SetScale : Scale cannot be negative!");
				return;
			}

			m_scale = a_scale;
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			t_bodyInterface->DeactivateBody(m_body->GetID());

			const JPH::Vec3 t_halfExtent = { a_scale.x * 0.5f, a_scale.y * 0.5f, a_scale.z * 0.5f };
			JPH::Shape* newShape = new JPH::BoxShape(t_halfExtent);

			t_bodyInterface->SetShape(m_body->GetID(), newShape, true, JPH::EActivation::Activate); // 'true' means update mass properties

		}

		void RigidBody::SetRadius(float a_radius)
		{
			if (m_colliderType == ColliderType::BOX)
			{
				LOG_ERROR("RigidBody::SetRadius : Cannot set radius on box collider!");
				return;
			}
			if (a_radius < 0.f)
			{
				LOG_ERROR("RigidBody::SetRadius : Radius cannot be negative!");
				return;
			}
			m_radius = a_radius;

			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			t_bodyInterface->DeactivateBody(m_body->GetID());
			JPH::Shape* newShape;
			switch (m_colliderType)
			{
			case ColliderType::BOX:
				break;
			case ColliderType::SPHERE:
				newShape = new JPH::SphereShape(m_radius);
				break;
			case ColliderType::CAPSULE:
				newShape = new JPH::CapsuleShape(m_halfHeight, m_radius);
				break;
			}
			t_bodyInterface->SetShape(m_body->GetID(), newShape, true, JPH::EActivation::Activate); // 'true' means update mass properties

		}

		void RigidBody::SetHalfHeight(float a_halfHeight)
		{
			if (m_colliderType != ColliderType::CAPSULE)
			{
				LOG_ERROR("RigidBody::SetRadius : Cannot set halfheight on non-capsule collider!");
				return;
			}
			if (a_halfHeight < 0.f)
			{
				LOG_ERROR("RigidBody::SetHalfHeight : Halfheight cannot be negative!");
				return;
			}
			m_halfHeight = a_halfHeight;

			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			
			t_bodyInterface->DeactivateBody(m_body->GetID());

            JPH::Shape* newShape =  new JPH::CapsuleShape(m_halfHeight, m_radius);

			t_bodyInterface->SetShape(m_body->GetID(), newShape, true, JPH::EActivation::Activate); // 'true' means update mass properties
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

		void RigidBody::LockRotation(const char a_axis)
		{
			if (m_lockedRotationAxes.contains(a_axis))
			{
				LOG_WARNING("Rotation axis: " + std::string(1, a_axis) + " is already locked!");
				return;
			}

			JPH::SixDOFConstraintSettings t_constraintSettings;
			t_constraintSettings.mPosition1 = JPH::Vec3(0, 0, 0);
			t_constraintSettings.mPosition2 = JPH::Vec3(0, 0, 0);

			switch (a_axis)
			{
			case 'x':
				t_constraintSettings.SetLimitedAxis(JPH::SixDOFConstraintSettings::EAxis::RotationX, 1.f, 0.f);
				break;
			case 'y':
				t_constraintSettings.SetLimitedAxis(JPH::SixDOFConstraintSettings::EAxis::RotationY, 1.f, 0.f);
				break;
			case 'z':
				t_constraintSettings.SetLimitedAxis(JPH::SixDOFConstraintSettings::EAxis::RotationZ, 1.f, 0.f);
				break;
			default:
				LOG_ERROR("Invalid axis! Use 'x', 'y', or 'z'.");
				return;
			}

			GamePlay::PhysicsSystem* t_physicsSystem = GamePlay::ServiceLocator::GetPhysicsSystem();

			JPH::SixDOFConstraint* t_constraint = new JPH::SixDOFConstraint(*m_body, *t_physicsSystem->GetDummy(), t_constraintSettings);

			t_physicsSystem->AddConstraint(t_constraint);

			m_lockedRotationAxes.emplace(a_axis, t_constraint);
		}

		void RigidBody::UnlockRotation(const char a_axis)
		{
			const auto t_it = m_lockedRotationAxes.find(a_axis);
			if (t_it == m_lockedRotationAxes.end())
			{
				LOG_WARNING("Rotation axis: " + std::string(1, a_axis) + " is already unlocked!");
				return;
			}

			GamePlay::ServiceLocator::GetPhysicsSystem()->RemoveConstraint(t_it->second);

			m_lockedRotationAxes.erase(t_it);
		}

		void RigidBody::SetFriction(float a_friction)
		{
			m_friction = a_friction;
			m_body->SetFriction(a_friction);
		}

		void RigidBody::SetRestitution(float a_restitution)
		{
			m_restitution = a_restitution;
			m_body->SetRestitution(a_restitution);
		}

		void RigidBody::SetColliderType(ColliderType a_type)
		{
			if (a_type == m_colliderType)
				return;
			m_colliderType = a_type;
			switch (a_type)
			{
			case ColliderType::BOX:
				{
				JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

				t_bodyInterface->DeactivateBody(m_body->GetID());

				const JPH::Vec3 t_halfExtent = { m_scale.x * 0.5f, m_scale.y * 0.5f, m_scale.z * 0.5f };
				m_shape = new JPH::BoxShape(t_halfExtent);

				t_bodyInterface->SetShape(m_body->GetID(), m_shape, true, JPH::EActivation::Activate); // 'true' means update mass properties
				}
			case ColliderType::CAPSULE:
			{
				JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
				t_bodyInterface->DeactivateBody(m_body->GetID());
				m_shape = new JPH::CapsuleShape(m_halfHeight, m_radius);
				t_bodyInterface->SetShape(m_body->GetID(), m_shape, true, JPH::EActivation::Activate); // 'true' means update mass properties
			}
			case ColliderType::SPHERE:
			{
				JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
				t_bodyInterface->DeactivateBody(m_body->GetID());
				m_shape = new JPH::SphereShape(m_radius);
				t_bodyInterface->SetShape(m_body->GetID(), m_shape, true, JPH::EActivation::Activate); // 'true' means update mass properties
			}
			}
		}

		void RigidBody::SetObjectLayer(CollisionLayer a_layer)
		{
			m_collisionLayer = a_layer;
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			t_bodyInterface->DeactivateBody(m_body->GetID());
			t_bodyInterface->RemoveBody(m_body->GetID());

			t_bodyInterface->SetObjectLayer(m_body->GetID(),CollisionLayerToJPHLayer(a_layer));

			t_bodyInterface->AddBody(m_body->GetID(), JPH::EActivation::Activate);
		}

		void RigidBody::SetBodyType(BodyType a_type)
		{
			m_bodyType = a_type;
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();

			t_bodyInterface->DeactivateBody(m_body->GetID());
			t_bodyInterface->SetMotionType(m_body->GetID(), BodyTypeToJPHType(a_type), JPH::EActivation::Activate);
			t_bodyInterface->ActivateBody(m_body->GetID());
			}

		bool RigidBody::IsRotLockedX() const
		{
			const auto t_it = m_lockedRotationAxes.find('x');
			return t_it != m_lockedRotationAxes.end();
		}

		bool RigidBody::IsRotLockedY() const
		{
			const auto t_it = m_lockedRotationAxes.find('y');
			return t_it != m_lockedRotationAxes.end();
		}

		bool RigidBody::IsRotLockedZ() const
		{
			const auto t_it = m_lockedRotationAxes.find('z');
			return t_it != m_lockedRotationAxes.end();
		}

		JPH::BodyID RigidBody::GetBodyID() const
		{
			return m_body->GetID();
		}

		/**
		 * Function returning a bool to check if an object is 'grounded' or not. Does a raycast towards the ground
		 * Needs the object's position and rotation to correctly set the starting position.
		 */
		bool RigidBody::IsGrounded(Math::vec3 a_pos, Math::quat a_rot)
		{
			JPH::Vec3 jphPosition(a_pos.x, a_pos.y, a_pos.z);
			JPH::Quat jphRotation(a_rot.x, a_rot.y, a_rot.z, a_rot.w);

			JPH::Mat44 startTransform = JPH::Mat44::sRotationTranslation(jphRotation, jphPosition);


			JPH::RShapeCast t_shapeCast
			{
				m_shape,
				{1,1,1},
				startTransform,
				{0,-1,0}
			};

			JPH::ShapeCastSettings t_filter;
			IgnoreBodyFilter ignoreSelfFilter(m_body->GetID());
			JPH::BroadPhaseLayerFilter t_layerFilter; //blank elements we need so i can access the BodyFilter of the CastShape function
			JPH::ObjectLayerFilter t_objectLayerFilter;
			JPH::ClosestHitCollisionCollector<JPH::CastShapeCollector> t_collector;

			JPH::PhysicsSystem* t_physicsSystem = GamePlay::ServiceLocator::GetPhysicsSystem()->GetPhysicsSystem();
			//this thing was genuinely torturous. Terrible documentation and just look at all the stuff i had to do
			t_physicsSystem->GetNarrowPhaseQuery().CastShape(t_shapeCast, t_filter, JPH::Vec3(0,0,0), t_collector, t_layerFilter,t_objectLayerFilter, ignoreSelfFilter);

			if (t_collector.HadHit()) {
				const auto& t_hit = t_collector.mHit;
				if (t_hit.mFraction < 0.1f)
					return true;
			}
			return false;
		}

		void RigidBody::Remove() const
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			t_bodyInterface->RemoveBody(m_body->GetID());
		}

		void RigidBody::Destroy()
		{
			std::vector<char> t_axesToUnlock;
			for (const auto& t_axis : m_lockedRotationAxes | std::views::keys)
			{
				t_axesToUnlock.push_back(t_axis);
			}

			for (const char t_axis : t_axesToUnlock)
			{
				UnlockRotation(t_axis);
			}
			m_lockedRotationAxes.clear();
			t_axesToUnlock.clear();

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

		void RigidBody::DestroyBody()
		{
			JPH::BodyInterface* t_bodyInterface = GamePlay::ServiceLocator::GetPhysicsSystem()->GetBodyInterface();
			t_bodyInterface->RemoveBody(m_body->GetID());
			t_bodyInterface->DestroyBody(m_body->GetID());
			m_body = nullptr;
		}
	}
}
