#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "EngineExport.h"

#include "Ressources/ResourceManager.h"
#include "GamePlay/Components/ComponentsPool.h"

namespace Engine
{
	namespace GamePlay
	{
		class ServiceLocator
		{
		public:
			static void ProvideResourceManager(Resource::ResourceManager* a_resourceManager);
			static void ProvideComponentsPool(ComponentsPool* a_componentsPool);

			static Resource::ResourceManager* GetResourceManager();
			static ComponentsPool* GetComponentsPool();

		private:
			static Resource::ResourceManager* m_resourceManager;
			static ComponentsPool* m_componentsPool;
		};
	}
}

#endif