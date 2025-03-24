#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "EngineExport.h"

#include "Ressources/ResourceManager.h"
#include "GamePlay/ComponentsPool.h"

namespace Engine
{
	namespace GamePlay
	{
		class ServiceLocator
		{
		public:
			

		private:
			static Resources::ResourceManager* m_resourceManager = nullptr;
			static ComponentsPool* m_componentsPool = nullptr;
		};
	}
}

#endif