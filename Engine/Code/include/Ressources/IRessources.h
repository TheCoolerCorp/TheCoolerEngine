#ifndef IRESSOURCES_H
#define IRESSOURCES_H

#include "EngineExport.h"

namespace Engine
{
	namespace Ressources
	{
		class IRessource
		{
		public:
			ENGINE_API virtual void Create();
			ENGINE_API virtual void Destroy();
		};
	}
}

#endif