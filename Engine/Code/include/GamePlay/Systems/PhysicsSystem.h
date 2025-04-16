#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "EngineExport.h"

#include "System.h"
#include "GamePlay/Components/RigidBodyComponent.h"
#include "Jolt/Core/JobSystemThreadPool.h"

namespace Engine
{
	namespace GamePlay
	{
		class PhysicsSystem : System
		{
		public:
			ENGINE_API PhysicsSystem() = default;
			ENGINE_API ~PhysicsSystem() override = default;

			ENGINE_API void Create();
			ENGINE_API void Update(const float a_deltaTime, const std::vector<Math::Transform*>& a_transforms);
			ENGINE_API void Destroy();

			ENGINE_API uint32_t AddComponent(RigidBodyComponent* a_component);
			ENGINE_API RigidBodyComponent* GetComponent(uint32_t a_id) const;
			ENGINE_API void RemoveComponent(uint32_t a_id);

			ENGINE_API uint32_t GetSize() const { return static_cast<uint32_t>(m_components.size()); }

			ENGINE_API JPH::BodyInterface* GetBodyInterface() const { return m_bodyInterface; }

		private:
			std::vector<RigidBodyComponent*> m_components{};
			std::vector<uint32_t> m_availableIds{};
			JPH::PhysicsSystem m_physicsSystem{};
			JPH::BodyInterface* m_bodyInterface = nullptr;

			const JPH::uint m_maxBodies = 65536;
			const JPH::uint m_numBodyMutexes = 0;
			const JPH::uint m_maxBodyPairs = 65536;
			const JPH::uint m_maxContactConstraints = 10240;
			JPH::MyBroadPhaseLayerInterface m_broadPhaseLayerInterface;
			JPH::ObjectVsBroadPhaseLayerFilterImpl m_objectVsBroadphaseLayerFilter;
			JPH::ObjectLayerPairFilterImpl m_objectVsObjectLayerFilter;
			JPH::MyContactListener m_contactListener;
			JPH::TempAllocatorImpl* m_tempAllocator = nullptr;
			JPH::JobSystemThreadPool* m_jobSystem = nullptr;
		};
	}
}

#endif