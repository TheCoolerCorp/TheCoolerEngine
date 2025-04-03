#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		Resource::ResourceManager* ServiceLocator::m_resourceManager = nullptr;
		TransformSystem* ServiceLocator::m_transformSystem = nullptr;
		MeshRendererSystem* ServiceLocator::m_meshRendererSystem = nullptr;

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


		Resource::ResourceManager* ServiceLocator::GetResourceManager()
		{
			return m_resourceManager;
		}

		TransformSystem* ServiceLocator::GetTransformSystem()
		{
			return m_transformSystem;
		}

		TransformSystem* ServiceLocator::GetMeshRendererSystem()
		{
			return m_meshRendererSystem;
		}
	}
}