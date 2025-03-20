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

			ENGINE_API virtual void Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool) = 0;
			ENGINE_API virtual void Destroy(Core::RHI::ILogicalDevice* a_logicalDevice) = 0;
		};
	}
}

#endif