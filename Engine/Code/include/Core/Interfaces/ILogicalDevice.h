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

			class ENGINE_API ILogicalDevice
			{
			public:
				virtual ~ILogicalDevice() = default;
				virtual void Create(IPhysicalDevice* a_physicalDevice, ISurface* a_surface) = 0;
				virtual void Destroy() = 0;
				virtual GraphicsAPI::VulkanLogicalDevice* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanDevice!"); return nullptr; }
			};
		}
	}
}

#endif