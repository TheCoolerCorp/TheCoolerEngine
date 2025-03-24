#ifndef IRESOURCE_H
#define IRESOURCE_H

#include "EngineExport.h"

#include <string>
namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class ApiInterface;
			class ILogicalDevice;
			class IPhysicalDevice;
			class ICommandPool;
		}
	}

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