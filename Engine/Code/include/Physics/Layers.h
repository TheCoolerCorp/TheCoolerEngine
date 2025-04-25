#ifndef LAYERS_H
#define LAYERS_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "Core/Logger/Logger.h"
#include "Core/Assertion/Assertion.h"
#include "EngineExport.h"

JPH_NAMESPACE_BEGIN

namespace Layers
{
    static constexpr uint8 NON_MOVING = 0;
    static constexpr uint8 MOVING = 1;
    static constexpr uint8 TRIGGER = 2;
    static constexpr uint8 DISABLED = 4;
    static constexpr uint8 NUM_LAYERS = 5;
}

class ObjectLayerPairFilterImpl final : public ObjectLayerPairFilter
{
public:
    ENGINE_API [[nodiscard]] bool ShouldCollide(ObjectLayer a_inObject1, ObjectLayer a_inObject2) const override;
};

namespace BroadPhaseLayers
{
    static constexpr BroadPhaseLayer NON_MOVING(0);
    static constexpr BroadPhaseLayer MOVING(1);
    static constexpr BroadPhaseLayer TRIGGER(2);
    static constexpr BroadPhaseLayer DISABLED(3);
    static constexpr uint32_t NUM_LAYERS(4);
}

class MyBroadPhaseLayerInterface final : public BroadPhaseLayerInterface
{
public:
    ENGINE_API MyBroadPhaseLayerInterface();

    ENGINE_API [[nodiscard]] uint32_t GetNumBroadPhaseLayers() const override;

    ENGINE_API [[nodiscard]] BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer a_iLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    ENGINE_API [[nodiscard]] const char* GetBroadPhaseLayerName(BroadPhaseLayer a_inLayer) const override;
#endif

private:
    BroadPhaseLayer m_objectToBroadPhase[Layers::NUM_LAYERS]{};
};

class ObjectVsBroadPhaseLayerFilterImpl final : public ObjectVsBroadPhaseLayerFilter
{
public:
    ENGINE_API [[nodiscard]] bool ShouldCollide(ObjectLayer a_inLayer1, BroadPhaseLayer a_inLayer2) const override;
};

JPH_NAMESPACE_END

#endif