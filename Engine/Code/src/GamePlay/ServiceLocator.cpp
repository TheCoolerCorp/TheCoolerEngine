#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		Resource::ResourceManager* ServiceLocator::m_resourceManager = nullptr;
		ComponentsPool* ServiceLocator::m_componentsPool = nullptr;

		void ServiceLocator::ProvideResourceManager(Resource::ResourceManager* a_resourceManager)
		{
			m_resourceManager = a_resourceManager;
		}

		void ServiceLocator::ProvideComponentsPool(ComponentsPool* a_componentsPool)
		{
			m_componentsPool = a_componentsPool;
		}


		Resource::ResourceManager* ServiceLocator::GetResourceManager()
		{
			return m_resourceManager;
		}

		ComponentsPool* ServiceLocator::GetComponentsPool()
		{
			return m_componentsPool;
		}
	}
}