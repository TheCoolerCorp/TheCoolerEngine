#include "GamePlay/Systems/PhysicsSystem.h"

#include "Jolt/RegisterTypes.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"

#include "GamePlay/Others/Scene.h"

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

			const JPH::BodyCreationSettings t_dummySettings(
				new JPH::BoxShape(JPH::Vec3(0.1f, 0.1f, 0.1f)),
				JPH::Vec3::sZero(),
				JPH::Quat::sIdentity(),
				JPH::EMotionType::Static,
				JPH::Layers::DISABLED
			);
			m_dummy = m_bodyInterface->CreateBody(t_dummySettings);
			m_bodyInterface->AddBody(m_dummy->GetID(), JPH::EActivation::DontActivate);
		}

		void PhysicsSystem::Update(const float a_deltaTime, Scene* a_scene)
		{
			constexpr float t_stepSize = 1.0f / 60.0f;
			const int t_collisionSteps = static_cast<int>(ceil(a_deltaTime / t_stepSize));

			if (t_collisionSteps > 60)
			{
				return;
			}

			UpdateEnqueued();

			m_physicsSystem.Update(a_deltaTime, t_collisionSteps, m_tempAllocator, m_jobSystem);

			UpdateTransforms(a_scene);
		}

		void PhysicsSystem::Destroy()
		{
			m_bodyInterface->RemoveBody(m_dummy->GetID());
			m_bodyInterface->DestroyBody(m_dummy->GetID());
			m_dummy = nullptr;

			for (RigidBodyComponent* t_component : m_components)
			{
				if (t_component == nullptr)
					continue;
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

		void PhysicsSystem::RemoveAllComponents()
		{
			for (RigidBodyComponent* t_component : m_components)
			{
				t_component->Destroy();
				delete t_component;
			}
			m_components.clear();
			m_availableIds.clear();
		}

		void PhysicsSystem::EnqueueLinearVelocity(JPH::BodyID a_bodyID, Math::vec3 a_linearVelocity)
		{
			m_linearVelocityQueue.emplace_back(a_bodyID, a_linearVelocity);
		}

		void PhysicsSystem::EnqueueAddForce(JPH::BodyID a_bodyID, Math::vec3 a_force)
		{
			m_addForceQueue.emplace_back(a_bodyID, a_force);
		}

		void PhysicsSystem::EnqueueAddImpulse(JPH::BodyID a_bodyID, Math::vec3 a_force)
		{
			m_addImpulseQueue.emplace_back(a_bodyID, a_force);
		}

		void PhysicsSystem::AddConstraint(JPH::Constraint* a_constraint)
		{
			m_physicsSystem.AddConstraint(a_constraint);
		}

		void PhysicsSystem::RemoveConstraint(JPH::Constraint* a_constraint)
		{
			m_physicsSystem.RemoveConstraint(a_constraint);
		}

		void PhysicsSystem::NotifyCollision(const JPH::CollisionEvent a_collisionEvent, JPH::BodyID a_body1,
		                                    JPH::BodyID a_body2)
		{
			auto t_unaryPredicate1 = [&a_body1](const RigidBodyComponent* a_rigidBodyComponent)
			{
				return a_body1 == a_rigidBodyComponent->GetBody()->GetBodyID();
			};
			const auto t_it1 = std::ranges::find_if(m_components, t_unaryPredicate1);

			auto t_unaryPredicate2 = [&a_body2](const RigidBodyComponent* a_rigidBodyComponent)
			{
				return a_body2 == a_rigidBodyComponent->GetBody()->GetBodyID();
			};
			const auto t_it2 = std::ranges::find_if(m_components, t_unaryPredicate2);

			if (t_it1 == m_components.end() || t_it2 == m_components.end())
			{
				return;
			}
			RigidBodyComponent* t_rigidBodyComponent1 = *t_it1;
			RigidBodyComponent* t_rigidBodyComponent2 = *t_it2;

			t_rigidBodyComponent1->NotifyCollision(a_collisionEvent, t_rigidBodyComponent2);
			t_rigidBodyComponent2->NotifyCollision(a_collisionEvent, t_rigidBodyComponent1);
		}

		void PhysicsSystem::NotifyCollisionExit(JPH::BodyID a_body1, JPH::BodyID a_body2)
		{
			auto t_unaryPredicate1 = [&a_body1](const RigidBodyComponent* a_rigidBodyComponent)
				{
					return a_body1 == a_rigidBodyComponent->GetBody()->GetBodyID();
				};
			const auto t_it1 = std::ranges::find_if(m_components, t_unaryPredicate1);

			auto t_unaryPredicate2 = [&a_body2](const RigidBodyComponent* a_rigidBodyComponent)
				{
					return a_body2 == a_rigidBodyComponent->GetBody()->GetBodyID();
				};
			const auto t_it2 = std::ranges::find_if(m_components, t_unaryPredicate2);

			if (t_it1 == m_components.end() || t_it2 == m_components.end())
			{
				return;
			}
			RigidBodyComponent* t_rigidBodyComponent1 = *t_it1;
			RigidBodyComponent* t_rigidBodyComponent2 = *t_it2;

			const JPH::CollisionEvent t_collisionEvent = t_rigidBodyComponent1->IsTrigger() || t_rigidBodyComponent2->IsTrigger() ? JPH::CollisionEvent::TRIGGER_EXIT : JPH::CollisionEvent::COLLISION_EXIT;

			t_rigidBodyComponent1->NotifyCollision(t_collisionEvent, t_rigidBodyComponent2);
			t_rigidBodyComponent2->NotifyCollision(t_collisionEvent, t_rigidBodyComponent1);
		}

		void PhysicsSystem::UpdatesFromTransforms(Scene* a_scene) const
		{
			for (size_t i = 0; i < m_components.size(); ++i)
			{
				if (m_components[i] == nullptr)
					continue;
				//To ensure correct access, we get the transform directly from the scene by getting the parent GameObject
				m_components[i]->UpdateFromTransform(a_scene->GetGameObject(m_components[i]->GetGameObjectID())->GetComponent<TransformComponent>()->GetTransform());
			}
		}

		void PhysicsSystem::UpdateTransforms(Scene* a_scene) const
		{
			for (size_t i = 0; i < m_components.size(); ++i)
			{
				if (m_components[i] == nullptr)
					continue;
				m_components[i]->UpdateObjectTransform(a_scene->GetGameObject(m_components[i]->GetGameObjectID())->GetComponent<TransformComponent>()->GetTransform());
			}
		}

		void PhysicsSystem::UpdateEnqueued()
		{
			for (auto& [fst, snd] : m_linearVelocityQueue)
			{
				const JPH::Vec3 t_velocity = { snd.x, snd.y, snd.z };
				m_bodyInterface->SetLinearVelocity(fst, t_velocity);
			}
			m_linearVelocityQueue.clear();

			for (auto& [fst, snd] : m_addForceQueue)
			{
				const JPH::Vec3 t_force = { snd.x, snd.y, snd.z };
				m_bodyInterface->AddForce(fst, t_force);
			}
			m_addForceQueue.clear();

			for (auto& [fst, snd] : m_addImpulseQueue)
			{
				const JPH::Vec3 t_impulse = { snd.x, snd.y, snd.z };
				m_bodyInterface->AddImpulse(fst, t_impulse);
			}
			m_addImpulseQueue.clear();
		}
	}
}
