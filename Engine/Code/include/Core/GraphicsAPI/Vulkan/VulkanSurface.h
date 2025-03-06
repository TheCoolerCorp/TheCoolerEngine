#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>

#include "Core/Interfaces/ISurface.h"


namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanSurface : public RHI::ISurface
			{
			public:
				void Create(Window::IWindow* a_window, RHI::IInstance* a_instance) override;
				void Destroy(RHI::IInstance* a_instance) override;
				VulkanSurface* CastVulkan() override { return this; }
				VkSurfaceKHR GetVkSurfaceKHR() { return m_surface; }

			private:
				VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			};
		}
	}
}

#endif