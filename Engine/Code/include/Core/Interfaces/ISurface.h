#ifndef ISURFACE_H
#define ISURFACE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			class IWindow;
		}

		namespace GraphicsAPI
		{
			class VulkanSurface;
		}

		namespace RHI
		{
			class IInstance;
			class IPhysicalDevice;

			class ISurface
			{
			public:
				ENGINE_API virtual ~ISurface() = default;
				ENGINE_API virtual void Create(Window::IWindow* window, IInstance* instance) = 0;
				ENGINE_API virtual void SetupInfo(IPhysicalDevice* a_physicalDevice) {};
				ENGINE_API virtual void Destroy(IInstance* instance) = 0;
				ENGINE_API virtual GraphicsAPI::VulkanSurface* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanSurface!"); return nullptr; }

			};
		}
	}
}

#endif