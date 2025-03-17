#ifndef VULKANLOGICALDEVICE_H
#define VULKANLOGICALDEVICE_H

#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Interfaces/ILogicalDevice.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanLogicalDevice : public RHI::ILogicalDevice
			{
			public:
				ENGINE_API ~VulkanLogicalDevice() override = default;
				ENGINE_API void Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface) override;
				ENGINE_API void Destroy() override;
				ENGINE_API VulkanLogicalDevice* CastVulkan() override { return this; }
				ENGINE_API [[nodiscard]] VkDevice GetVkDevice() const { return m_device; }
				ENGINE_API VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
				ENGINE_API VkQueue GetPresentQueue() const { return m_presentQueue; }

			private:
				VkDevice m_device = VK_NULL_HANDLE;
				VkQueue m_graphicsQueue{};
				VkQueue m_presentQueue{};

			};
		}
	}
}

#endif