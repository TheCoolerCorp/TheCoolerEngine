#ifndef IPHYSICALDEVICE_H
#define IPHYSICALDEVICE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanPhysicalDevice;
		}

		namespace RHI
		{
			class IInstance;
			class ISurface;

			class IPhysicalDevice
			{
			public:
				ENGINE_API virtual ~IPhysicalDevice() = default;
				ENGINE_API virtual void Create(IInstance* instance, ISurface* a_surface) = 0;
				ENGINE_API virtual void Destroy() = 0;
				ENGINE_API virtual GraphicsAPI::VulkanPhysicalDevice* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }
			};
		}
	}
}

#endif