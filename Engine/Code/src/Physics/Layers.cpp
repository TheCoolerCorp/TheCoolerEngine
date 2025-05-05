#include "Physics/Layers.h"

#include "Physics/RigidBody.h"

JPH_NAMESPACE_BEGIN
	bool ObjectLayerPairFilterImpl::ShouldCollide(const ObjectLayer a_inObject1, const ObjectLayer a_inObject2) const
	{
		switch (a_inObject1)
		{
		case Layers::NON_MOVING:
			return a_inObject2 == Layers::MOVING;
		case Layers::MOVING:
			return a_inObject2 != Layers::DISABLED;
		case Layers::TRIGGER:
			return a_inObject2 != Layers::TRIGGER;
		case Layers::DISABLED:
			return false;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}

	MyBroadPhaseLayerInterface::MyBroadPhaseLayerInterface()
	{
		m_objectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		m_objectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	uint32_t MyBroadPhaseLayerInterface::GetNumBroadPhaseLayers() const
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	BroadPhaseLayer MyBroadPhaseLayerInterface::GetBroadPhaseLayer(const ObjectLayer a_iLayer) const
	{
		JPH_ASSERT(a_iLayer < Layers::NUM_LAYERS);
		return m_objectToBroadPhase[a_iLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char* MyBroadPhaseLayerInterface::GetBroadPhaseLayerName(const BroadPhaseLayer a_inLayer) const
	{
		switch (static_cast<BroadPhaseLayer::Type>(a_inLayer))
		{
		case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::NON_MOVING):	
			return "NON_MOVING";
		case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::MOVING):		
			return "MOVING";
		case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::TRIGGER):
			return "TRIGGER";
		case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::DISABLED):
			return "DISABLED";
		default:													
			JPH_ASSERT(false);
			return "INVALID";
		}
	}
#endif

	bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(const ObjectLayer a_inLayer1, const BroadPhaseLayer a_inLayer2) const
	{
		switch (a_inLayer1)
		{
		case Layers::NON_MOVING:
			return a_inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return a_inLayer2 != BroadPhaseLayers::DISABLED;
		case Layers::TRIGGER:
			return a_inLayer2 != BroadPhaseLayers::DISABLED;
		case Layers::DISABLED:
			return false;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}

JPH_NAMESPACE_END
