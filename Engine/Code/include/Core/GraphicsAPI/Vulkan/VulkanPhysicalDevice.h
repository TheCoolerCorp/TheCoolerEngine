#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

#include "Core/Interfaces/IPhysicalDevice.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanPhysicalDevice : public RHI::IPhysicalDevice
			{
			public:
				ENGINE_API void Create(RHI::IInstance* a_instance, RHI::ISurface* a_surface) override;
				ENGINE_API void Destroy() override {}
				ENGINE_API VulkanPhysicalDevice* CastVulkan() override { return this; }
				ENGINE_API VkPhysicalDevice GetVkPhysicalDevice() const { return m_physicalDevice; }

				ENGINE_API VkFormat FindSupportedFormat(const std::vector<VkFormat>& a_candidates, VkImageTiling a_tiling, VkFormatFeatureFlags a_features) const;

			private:
				void PickPhysicalDevice(VkInstance a_instance, VkSurfaceKHR a_surface);
				ENGINE_API static bool IsPhysicalDeviceSuitable(VkPhysicalDevice a_physicalDevice, VkSurfaceKHR a_surface);
				ENGINE_API static uint32_t RatePhysicalDevice(VkPhysicalDevice a_physicalDevice, VkSurfaceKHR a_surface);

				VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
			};
		}
	}
}

#endif