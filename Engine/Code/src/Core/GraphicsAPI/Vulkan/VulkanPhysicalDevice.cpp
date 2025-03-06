#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"

#include "Core/Assertion/Assertion.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/Interfaces/IInstance.h"
#include <Core/GraphicsAPI/Vulkan/QueueFamilies.h>
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include <map>

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanPhysicalDevice::Create(RHI::IInstance* a_instance, RHI::ISurface* a_surface)
			{
				PickPhysicalDevice(a_instance->CastVulkan()->GetVkInstance(), a_surface->CastVulkan()->GetVkSurfaceKHR());
			}

			void VulkanPhysicalDevice::PickPhysicalDevice(const VkInstance a_instance, VkSurfaceKHR a_surface)
			{
				uint32_t t_physicalDeviceCount = 0;
				vkEnumeratePhysicalDevices(a_instance, &t_physicalDeviceCount, nullptr);

				ASSERT(t_physicalDeviceCount != 0, "failed to find GPUs with Vulkan support!");

				std::vector<VkPhysicalDevice> t_physicalDevices(t_physicalDeviceCount);
				vkEnumeratePhysicalDevices(a_instance, &t_physicalDeviceCount, t_physicalDevices.data());

				std::multimap<uint32_t, VkPhysicalDevice> candidates;

				for (const auto& t_physicalDevice : t_physicalDevices) {
					uint32_t score = RatePhysicalDevice(t_physicalDevice, a_surface);
					candidates.insert(std::make_pair(score, t_physicalDevice));
				}

				ASSERT(candidates.rbegin()->first > 0, "failed to find a suitable GPU!");

				m_physicalDevice = candidates.rbegin()->second;

				candidates.clear();
				t_physicalDevices.clear();

			}

			uint32_t VulkanPhysicalDevice::RatePhysicalDevice(const VkPhysicalDevice a_physicalDevice, const VkSurfaceKHR a_surface)
			{
				VkPhysicalDeviceProperties t_physicalDeviceProperties;
				VkPhysicalDeviceFeatures t_physicalDeviceFeatures;
				vkGetPhysicalDeviceProperties(a_physicalDevice, &t_physicalDeviceProperties);
				vkGetPhysicalDeviceFeatures(a_physicalDevice, &t_physicalDeviceFeatures);

				if (!t_physicalDeviceFeatures.geometryShader || !IsPhysicalDeviceSuitable(a_physicalDevice, a_surface)) 
				{
					return 0;
				}

				uint32_t t_score = 0;

				if (t_physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) 
				{
					t_score += 1000;
				}

				t_score += t_physicalDeviceProperties.limits.maxImageDimension2D;

				return t_score;
			}

			bool VulkanPhysicalDevice::IsPhysicalDeviceSuitable(const VkPhysicalDevice a_physicalDevice, const VkSurfaceKHR a_surface)
			{
				const QueueFamilyIndices t_indices = QueueFamilyIndices::FindQueueFamilies(a_physicalDevice, a_surface);

				return t_indices.IsComplete();
			}
		}
	}
}
