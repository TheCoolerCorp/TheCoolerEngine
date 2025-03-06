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
				a_window->CreateWindowSurface(a_instance, this);
			}

			void VulkanSurface::SetupInfo(RHI::IPhysicalDevice* a_physicalDevice)
			{
				VkPhysicalDevice t_device = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();

				VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(t_device, m_surface, &m_info.capabilities), "Failed to get Surface capabilities");

				uint32_t formatCount;
				VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(t_device, m_surface, &formatCount, nullptr), "Failed to get Surface capabilities");

				uint32_t presentModeCount;
				VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(t_device, m_surface, &presentModeCount, nullptr), "Failed to get Surface capabilities");

				if (formatCount != 0) {
					m_info.formats.resize(formatCount);
					VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(t_device, m_surface, &formatCount, m_info.formats.data()), "Failed to get Surface capabilities");
				}

				if (presentModeCount != 0) {
					m_info.presentModes.resize(presentModeCount);
					VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(t_device, m_surface, &presentModeCount, m_info.presentModes.data()), "Failed to get Surface capabilities");
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
