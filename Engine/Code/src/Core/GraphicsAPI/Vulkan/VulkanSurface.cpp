#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/Window/IWindow.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanSurface::Create(Window::IWindow* a_window, RHI::IInstance* a_instance) 
			{
				a_window->CreateWindowSurface(a_instance, this);
			}

			void VulkanSurface::Destroy(RHI::IInstance* a_instance) 
			{
				VkInstance t_instance = a_instance->CastVulkan()->GetVkInstance();
				vkDestroySurfaceKHR(t_instance, m_surface, nullptr);
			}
		}
	}
}
