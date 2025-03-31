#ifndef IRESOURCE_H
#define IRESOURCE_H

#include "EngineExport.h"

#include <string>
namespace Engine
{
	namespace Resource
	{
		class IResource
		{
		public:
			virtual ~IResource() = default;
		};
	}
}

#endif