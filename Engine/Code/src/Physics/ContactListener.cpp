#include "Physics/ContactListener.h"

#include <iostream>

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
		std::cout << "A contact was added" << '\n';
	}

	void MyContactListener::OnContactPersisted(const Body& a_inBody1, const Body& a_inBody2,
		const ContactManifold& a_inManifold, ContactSettings& a_ioSettings)
	{
		std::cout << "A contact was persisted" << '\n';
	}

	void MyContactListener::OnContactRemoved(const SubShapeIDPair& a_inSubShapePair)
	{
		std::cout << "A contact was removed" << '\n';
	}

JPH_NAMESPACE_END
