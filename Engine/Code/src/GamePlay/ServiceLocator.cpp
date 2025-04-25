#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		Resource::ResourceManager* ServiceLocator::m_resourceManager = nullptr;
		TransformSystem* ServiceLocator::m_transformSystem = nullptr;
		MeshRendererSystem* ServiceLocator::m_meshRendererSystem = nullptr;
		PhysicsSystem* ServiceLocator::m_physicsSystem = nullptr;

		void ServiceLocator::ProvideResourceManager(Resource::ResourceManager* a_resourceManager)
		{
			m_resourceManager = a_resourceManager;
		}

		void ServiceLocator::ProvideTransformSystem(TransformSystem* a_transformSystem)
		{
			m_transformSystem = a_transformSystem;
		}

		void ServiceLocator::ProvideRendererSystem(MeshRendererSystem* a_meshRendererSystem)
		{
			m_meshRendererSystem = a_meshRendererSystem;
		}

		void ServiceLocator::ProvidePhysicsSystem(PhysicsSystem* a_physicsSystem)
		{
			m_physicsSystem = a_physicsSystem;
		}

		Resource::ResourceManager* ServiceLocator::GetResourceManager()
		{
			return m_resourceManager;
		}

		TransformSystem* ServiceLocator::GetTransformSystem()
		{
			return m_transformSystem;
		}

		MeshRendererSystem* ServiceLocator::GetMeshRendererSystem()
		{
			return m_meshRendererSystem;
		}

		PhysicsSystem* ServiceLocator::GetPhysicsSystem()
		{
			return m_physicsSystem;
		}
	}
}