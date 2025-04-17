#include "GamePlay/Systems/PhysicsSystem.h"

#include "Jolt/RegisterTypes.h"

namespace Engine
{
	namespace GamePlay
	{
		void PhysicsSystem::Create()
		{
			JPH::RegisterDefaultAllocator();

			JPH::Factory::sInstance = new JPH::Factory();

			JPH::RegisterTypes();

			m_tempAllocator = new JPH::TempAllocatorImpl(100 * 1024 * 1024);

			uint32_t t_numThreads = std::max(1u, std::thread::hardware_concurrency() - 1u);
			m_jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, t_numThreads);

			m_physicsSystem.Init(m_maxBodies, m_numBodyMutexes, m_maxBodyPairs, m_maxContactConstraints, m_broadPhaseLayerInterface, m_objectVsBroadphaseLayerFilter, m_objectVsObjectLayerFilter);

			m_physicsSystem.SetContactListener(&m_contactListener);

			m_bodyInterface = &m_physicsSystem.GetBodyInterface();

			m_physicsSystem.OptimizeBroadPhase();
		}

		void PhysicsSystem::Update(const float a_deltaTime, const std::vector<Math::Transform*>& a_transforms)
		{
			constexpr float t_stepSize = 1.0f / 60.0f;
			const int t_collisionSteps = static_cast<int>(ceil(a_deltaTime / t_stepSize));

			if (t_collisionSteps > 60)
			{
				return;
			}

			m_physicsSystem.Update(a_deltaTime, t_collisionSteps, m_tempAllocator, m_jobSystem);

			for (size_t i = 0; i < m_components.size(); ++i)
			{
				m_components[i]->UpdateObjectTransform(a_transforms[i]);
			}
		}

		void PhysicsSystem::Destroy()
		{
			for (RigidBodyComponent* t_component : m_components)
			{
				t_component->Destroy();
				delete t_component;
			}
			m_components.clear();
			m_availableIds.clear();

			JPH::UnregisterTypes();

			delete m_jobSystem;
			m_jobSystem = nullptr;

			delete m_tempAllocator;
			m_tempAllocator = nullptr;

			delete JPH::Factory::sInstance;
			JPH::Factory::sInstance = nullptr;
		}

		uint32_t PhysicsSystem::AddComponent(RigidBodyComponent* a_component)
		{
			if (m_availableIds.empty())
			{
				m_components.emplace_back(a_component);
				return static_cast<uint32_t>(m_components.size()) - 1u;
			}
			for (const uint32_t t_availableIndex : m_availableIds)
			{
				if (m_components.at(t_availableIndex) == nullptr)
				{
					m_components.at(t_availableIndex) = a_component;
					return t_availableIndex;
				}
			}
			return -1;
		}

		RigidBodyComponent* PhysicsSystem::GetComponent(const uint32_t a_id) const
		{
			if (a_id >= m_components.size())
			{
				return nullptr;
			}
			return m_components.at(a_id);
		}

		void PhysicsSystem::RemoveComponent(const uint32_t a_id)
		{
			m_components[a_id]->Destroy();
			delete m_components[a_id];
			m_components[a_id] = nullptr;
			m_availableIds.push_back(a_id);
		}
	}
}
