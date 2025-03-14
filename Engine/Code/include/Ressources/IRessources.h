#ifndef IRESSOURCES_H
#define IRESSOURCES_H

#include "EngineExport.h"

namespace Engine
{
	namespace Ressources
	{
		class ENGINE_API IRessource
		{
		public:
			virtual void Create();
			virtual void Destroy();
		};
	}
}

#endif