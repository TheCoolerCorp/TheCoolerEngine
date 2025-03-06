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

			class ENGINE_API IPhysicalDevice
			{
			public:
				virtual ~IPhysicalDevice() = default;
				virtual void Create(IInstance* instance, ISurface* a_surface) = 0;
				virtual GraphicsAPI::VulkanPhysicalDevice* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }
			};
		}
	}
}

#endif