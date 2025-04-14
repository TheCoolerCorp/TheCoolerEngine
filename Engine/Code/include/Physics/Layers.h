#ifndef LAYERS_H
#define LAYERS_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ContactListener.h>

#include "Core/Logger/Logger.h"
#include "Core/Assertion/Assertion.h"

JPH_NAMESPACE_BEGIN

namespace Layers
{
    static constexpr uint8 NON_MOVING = 0;
    static constexpr uint8 MOVING = 1;
    static constexpr uint8 NUM_LAYERS = 2;
}

class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
{
public:
    virtual bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override;
};

namespace BroadPhaseLayers
{
    static constexpr BroadPhaseLayer NON_MOVING(0);
    static constexpr BroadPhaseLayer MOVING(1);
    static constexpr uint NUM_LAYERS(2);
}

class MyBroadPhaseLayerInterface final : public BroadPhaseLayerInterface
{
public:
    MyBroadPhaseLayerInterface();

    uint GetNumBroadPhaseLayers() const override;

    BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer i_layer) const override;

#ifdef (JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override;
#endif

private:
    BroadPhaseLayer objectToBroadPhase[Layers::NUM_LAYERS]{};
};

class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override;
};

JPH_NAMESPACE_END

#endif