#ifndef ICOMMANDPOOL_H
#define ICOMMANDPOOL_H

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanCommandPool;
		}

		namespace RHI
		{
			class ISurface;
			class ILogicalDevice;
			class IPhysicalDevice;

			class ENGINE_API ICommandPool
			{
			public:
				virtual ~ICommandPool() = default;
				virtual void Create(IPhysicalDevice* a_physicalDevice, ISurface* a_surface, ILogicalDevice* a_logicalDevice) = 0;
				virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif