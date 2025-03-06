#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"

#include <vector>

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			bool QueueFamilyIndices::IsComplete() const
			{
				return m_graphicsFamily.has_value() && m_presentFamily.has_value();
			}

			QueueFamilyIndices QueueFamilyIndices::FindQueueFamilies(const VkPhysicalDevice a_physicalDevice, const VkSurfaceKHR a_surface)
			{
                QueueFamilyIndices t_indices;

                uint32_t t_queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(a_physicalDevice, &t_queueFamilyCount, nullptr);

                std::vector<VkQueueFamilyProperties> t_queueFamilies(t_queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(a_physicalDevice, &t_queueFamilyCount, t_queueFamilies.data());

                int i = 0;
                for (const auto& t_queueFamily : t_queueFamilies) 
                {
                    if (t_queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
                    {
                        t_indices.m_graphicsFamily = i;
                    }

                    VkBool32 presentSupport = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(a_physicalDevice, i, a_surface, &presentSupport);

                    if (presentSupport) {
                        t_indices.m_presentFamily = i;
                    }


                    if (t_indices.IsComplete()) 
                    {
                        break;
                    }

                    ++i;
                }

                return t_indices;
			}
		}
	}
}
