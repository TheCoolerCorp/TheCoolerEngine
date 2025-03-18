#ifndef ILOGICALDEVICE_H
#define ILOGICALDEVICE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanLogicalDevice;
		}

		namespace RHI 
		{
			class IValidationLayers;
			class IPhysicalDevice;
			class ISurface;

			class ILogicalDevice
			{
			public:
				ENGINE_API virtual ~ILogicalDevice() = default;
				ENGINE_API virtual void Create(IPhysicalDevice* a_physicalDevice, ISurface* a_surface) = 0;
				ENGINE_API virtual void WaitIdle() = 0;
				ENGINE_API virtual void Destroy() = 0;
				ENGINE_API virtual GraphicsAPI::VulkanLogicalDevice* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanDevice!"); return nullptr; }
			};
		}
	}
}

#endif