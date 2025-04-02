#ifndef SYSTEM_H
#define SYSTEM_H

#include "EngineExport.h"

#include "GamePlay/Components/ComponentsPool.h"

namespace Engine
{
	namespace GamePlay
	{
		class System
		{
		public:
			ENGINE_API System() = default;
			ENGINE_API virtual ~System() = default;
		};
	}
}
#endif