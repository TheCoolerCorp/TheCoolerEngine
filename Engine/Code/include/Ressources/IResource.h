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

			ENGINE_API virtual void Create(std::string a_path) = 0;
			ENGINE_API virtual void Destroy() = 0;
		};
	}
}

#endif