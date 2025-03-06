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

			class ENGINE_API ISurface
			{
			public:
				virtual ~ISurface() = default;
				virtual void Create(Window::IWindow* window, IInstance* instance) = 0;
				virtual void SetupInfo(IPhysicalDevice* a_physicalDevice) {};
				virtual void Destroy(IInstance* instance) = 0;
				virtual GraphicsAPI::VulkanSurface* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanSurface!"); return nullptr; }

			};
		}
	}
}

#endif