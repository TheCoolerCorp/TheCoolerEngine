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
		class Scene;

		class PhysicsSystem : System
		{
		public:
			ENGINE_API PhysicsSystem() = default;
			ENGINE_API ~PhysicsSystem() override = default;

			ENGINE_API void Create();
			ENGINE_API void Update(const float a_deltaTime, Scene* a_scene);
			ENGINE_API void UpdatesFromTransforms(Scene* a_scene) const;
			ENGINE_API void SetComponentsForPlay() const;
			ENGINE_API void Destroy();

			ENGINE_API uint32_t AddComponent(RigidBodyComponent* a_component);
			ENGINE_API RigidBodyComponent* GetComponent(uint32_t a_id) const;
			ENGINE_API void RemoveComponent(uint32_t a_id);
			ENGINE_API void RemoveAllComponents();

			ENGINE_API void EnqueueLinearVelocity(JPH::BodyID a_bodyID, Math::vec3 a_linearVelocity);
			ENGINE_API void EnqueueAddForce(JPH::BodyID a_bodyID, Math::vec3 a_force);
			ENGINE_API void EnqueueAddImpulse(JPH::BodyID a_bodyID, Math::vec3 a_force);

			ENGINE_API void AddConstraint(JPH::Constraint* a_constraint);
			ENGINE_API void RemoveConstraint(JPH::Constraint* a_constraint);

			ENGINE_API void NotifyCollision(JPH::CollisionEvent a_collisionEvent, JPH::BodyID a_body1, JPH::BodyID a_body2);
			ENGINE_API void NotifyCollisionExit(JPH::BodyID a_body1, JPH::BodyID a_body2);

			ENGINE_API [[nodiscard]] JPH::Body* GetDummy() const { return m_dummy; }
			ENGINE_API [[nodiscard]] uint32_t GetSize() const { return static_cast<uint32_t>(m_components.size()); }
			ENGINE_API [[nodiscard]] JPH::BodyInterface* GetBodyInterface() const { return m_bodyInterface; }

		private:
			void UpdateTransforms(const Scene* a_scene) const;
			void UpdateEnqueued();

			std::vector<RigidBodyComponent*> m_components{};
			std::vector<std::pair<JPH::BodyID, Math::vec3>> m_linearVelocityQueue{};
			std::vector<std::pair<JPH::BodyID, Math::vec3>> m_addForceQueue{};
			std::vector<std::pair<JPH::BodyID, Math::vec3>> m_addImpulseQueue{};
			std::vector<uint32_t> m_availableIds{};
			JPH::PhysicsSystem m_physicsSystem{};
			JPH::BodyInterface* m_bodyInterface = nullptr;

			JPH::Body* m_dummy = nullptr;

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