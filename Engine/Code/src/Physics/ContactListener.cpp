#include "Physics/ContactListener.h"

#include <iostream>

#include "Gameplay/ServiceLocator.h"
#include "Jolt/Physics/Body/Body.h"
#include "Physics/Layers.h"

JPH_NAMESPACE_BEGIN
	ValidateResult MyContactListener::OnContactValidate(const Body& a_inBody1, const Body& a_inBody2, RVec3Arg a_inBaseOffset,
	                                                    const CollideShapeResult& a_inCollisionResult)
	{
		if (a_inBody1.GetObjectLayer() == Layers::DISABLED || a_inBody2.GetObjectLayer() == Layers::DISABLED)
		{
			return ValidateResult::RejectContact;
		}
	
		return ValidateResult::AcceptContact;
	}

	void MyContactListener::OnContactAdded(const Body& a_inBody1, const Body& a_inBody2, const ContactManifold& a_inManifold,
		ContactSettings& a_ioSettings)
	{
		Engine::GamePlay::PhysicsSystem* t_physicsSystem = Engine::GamePlay::ServiceLocator::GetPhysicsSystem();

		if (a_inBody1.IsSensor() || a_inBody2.IsSensor())
		{
			t_physicsSystem->NotifyCollision(CollisionEvent::TRIGGER_ENTER, a_inBody1.GetID(), a_inBody2.GetID());
			return;
		}

		t_physicsSystem->NotifyCollision(CollisionEvent::COLLISION_ENTER, a_inBody1.GetID(), a_inBody2.GetID());
	}

	void MyContactListener::OnContactPersisted(const Body& a_inBody1, const Body& a_inBody2,
		const ContactManifold& a_inManifold, ContactSettings& a_ioSettings)
	{
		Engine::GamePlay::PhysicsSystem* t_physicsSystem = Engine::GamePlay::ServiceLocator::GetPhysicsSystem();

		if (a_inBody1.IsSensor() || a_inBody2.IsSensor())
		{
			t_physicsSystem->NotifyCollision(CollisionEvent::TRIGGER_STAY, a_inBody1.GetID(), a_inBody2.GetID());
			return;
		}

		t_physicsSystem->NotifyCollision(CollisionEvent::COLLISION_STAY, a_inBody1.GetID(), a_inBody2.GetID());
	}

	void MyContactListener::OnContactRemoved(const SubShapeIDPair& a_inSubShapePair)
	{
		Engine::GamePlay::PhysicsSystem* t_physicsSystem = Engine::GamePlay::ServiceLocator::GetPhysicsSystem();

		BodyID a_body1ID = a_inSubShapePair.GetBody1ID();
		BodyID a_body2ID = a_inSubShapePair.GetBody2ID();

		t_physicsSystem->NotifyCollisionExit(a_body1ID, a_body2ID);
	}

JPH_NAMESPACE_END
