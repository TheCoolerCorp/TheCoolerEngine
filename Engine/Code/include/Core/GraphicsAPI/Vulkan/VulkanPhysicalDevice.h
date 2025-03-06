#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

#include <vulkan/vulkan.h>

#include "Core/Interfaces/IPhysicalDevice.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanPhysicalDevice : public RHI::IPhysicalDevice
			{
			public:
				void Create(RHI::IInstance* a_instance) override;
				VulkanPhysicalDevice* CastVulkan() override { return this; }
				VkPhysicalDevice GetVkPhysicalDevice() const { return m_physicalDevice; }

			private:
				void PickPhysicalDevice(VkInstance a_instance);
				static bool IsPhysicalDeviceSuitable(VkPhysicalDevice a_physicalDevice);
				static uint32_t RatePhysicalDevice(VkPhysicalDevice a_physicalDevice);

				VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
			};
		}
	}
}

#endif