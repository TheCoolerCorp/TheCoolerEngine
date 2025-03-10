#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/Window/IWindow.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanSurface::Create(Window::IWindow* a_window, RHI::IInstance* a_instance) 
			{
				a_window->CastGLFW();
				a_window->CreateWindowSurface(a_instance, this);
			}

			void VulkanSurface::SetupInfo(RHI::IPhysicalDevice* a_physicalDevice)
			{
				VkPhysicalDevice t_device = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();

				VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(t_device, m_surface, &m_info.mCapabilities), "Failed to get Surface capabilities");

				uint32_t t_formatCount;
				VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(t_device, m_surface, &t_formatCount, nullptr), "Failed to get Surface capabilities");

				uint32_t t_presentModeCount;
				VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(t_device, m_surface, &t_presentModeCount, nullptr), "Failed to get Surface capabilities");

				if (t_formatCount != 0) {
					m_info.mFormats.resize(t_formatCount);
					VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(t_device, m_surface, &t_formatCount, m_info.mFormats.data()), "Failed to get Surface capabilities");
				}

				if (t_presentModeCount != 0) {
					m_info.mPresentModes.resize(t_presentModeCount);
					VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(t_device, m_surface, &t_presentModeCount, m_info.mPresentModes.data()), "Failed to get Surface capabilities");
				}
			}

			void VulkanSurface::Destroy(RHI::IInstance* a_instance) 
			{
				VkInstance t_instance = a_instance->CastVulkan()->GetVkInstance();
				vkDestroySurfaceKHR(t_instance, m_surface, nullptr);
			}
		}
	}
}
