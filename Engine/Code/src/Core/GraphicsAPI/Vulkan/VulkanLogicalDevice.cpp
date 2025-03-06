#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanLogicalDevice::Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface)
			{
                VkPhysicalDevice t_physicalDevice = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();
                VkSurfaceKHR t_surface = a_surface->CastVulkan()->GetVkSurfaceKHR();

                QueueFamilyIndices t_indices = QueueFamilyIndices::FindQueueFamilies(t_physicalDevice, t_surface);

                std::vector<VkDeviceQueueCreateInfo> t_queueCreateInfos;
                std::set<uint32_t> t_uniqueQueueFamilies = 
                {
                    t_indices.GetGraphicsFamily().value(),
                    t_indices.GetPresentFamily().value()
                };

                float t_queuePriority = 1.0f;
                for (uint32_t t_queueFamily : t_uniqueQueueFamilies) {
                    VkDeviceQueueCreateInfo queueCreateInfo{};
                    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queueCreateInfo.queueFamilyIndex = t_queueFamily;
                    queueCreateInfo.queueCount = 1;
                    queueCreateInfo.pQueuePriorities = &t_queuePriority;
                    t_queueCreateInfos.push_back(queueCreateInfo);
                }

                VkPhysicalDeviceFeatures t_deviceFeatures{};

                VkDeviceCreateInfo t_createInfo{};
                t_createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

                t_createInfo.queueCreateInfoCount = static_cast<uint32_t>(t_queueCreateInfos.size());
                t_createInfo.pQueueCreateInfos = t_queueCreateInfos.data();

                t_createInfo.pEnabledFeatures = &t_deviceFeatures;

                t_createInfo.enabledExtensionCount = 0;

                if (RHI::ENABLEVALIDATIONLAYERS) 
                {
                    t_createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
                    t_createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
                }
                else 
                {
                    t_createInfo.enabledLayerCount = 0;
                }

                //if (vkCreateDevice(t_physicalDevice, &t_createInfo, nullptr, &m_device) != VK_SUCCESS) {
                //    throw std::runtime_error("failed to create logical device!");
                //}
                VK_CHECK(vkCreateDevice(t_physicalDevice, &t_createInfo, nullptr, &m_device), "failed to create logical device!");

                vkGetDeviceQueue(m_device, t_indices.GetGraphicsFamily().value(), 0, &m_graphicsQueue);
                //vkGetDeviceQueue(m_device, t_indices.GetPresentFamily().value(), 0, &m_presentQueue);
			}

			void VulkanLogicalDevice::Destroy()
			{
				vkDestroyDevice(m_device, nullptr);
			}
		}
	}
}
