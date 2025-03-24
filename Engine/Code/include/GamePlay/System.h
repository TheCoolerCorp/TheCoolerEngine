#ifndef SYSTEM_H
#define SYSTEM_H

#include "EngineExport.h"

#include "ComponentsPool.h"

namespace Engine
{
	namespace GamePlay
	{
		class System
		{
		public:
			ENGINE_API System() = default;
			ENGINE_API ~System() = default;

			ENGINE_API virtual void Update(ComponentsPool& a_componentsPool) = 0;

		};
	}
}
#endif