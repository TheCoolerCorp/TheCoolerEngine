#include "Physics/ContactListener.h"

#include <iostream>

JPH_NAMESPACE_BEGIN
	ValidateResult MyContactListener::OnContactValidate(const Body& a_inBody1, const Body& a_inBody2, RVec3Arg a_inBaseOffset,
	                                                    const CollideShapeResult& a_inCollisionResult)
	{
		std::cout << "Contact validate callback" << '\n';

		return ContactListener::OnContactValidate(a_inBody1, a_inBody2, a_inBaseOffset, a_inCollisionResult);
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
