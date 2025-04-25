#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>

#include "EngineExport.h"

JPH_NAMESPACE_BEGIN

enum class CollisionEvent : uint8_t
{
	COLLISION_ENTER = 0,
	COLLISION_STAY,
	COLLISION_EXIT,
	TRIGGER_ENTER,
	TRIGGER_STAY,
	TRIGGER_EXIT
};

class MyContactListener final : public ContactListener
{
public:
	ENGINE_API ValidateResult OnContactValidate(const Body& a_inBody1, const Body& a_inBody2, RVec3Arg a_inBaseOffset, const CollideShapeResult& a_inCollisionResult) override;

	ENGINE_API void OnContactAdded(const Body& a_inBody1, const Body& a_inBody2, const ContactManifold& a_inManifold, ContactSettings& a_ioSettings) override;

	ENGINE_API void OnContactPersisted(const Body& a_inBody1, const Body& a_inBody2, const ContactManifold& a_inManifold, ContactSettings& a_ioSettings) override;

	ENGINE_API void OnContactRemoved(const SubShapeIDPair& a_inSubShapePair) override;
};

JPH_NAMESPACE_END

#endif