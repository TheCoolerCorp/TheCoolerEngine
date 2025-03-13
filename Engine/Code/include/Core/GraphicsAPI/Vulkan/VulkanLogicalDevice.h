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
			class ENGINE_API VulkanLogicalDevice : public RHI::ILogicalDevice
			{
			public:
				~VulkanLogicalDevice() override = default;
				void Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface) override;
				void Destroy() override;
				VulkanLogicalDevice* CastVulkan() override { return this; }
				[[nodiscard]] VkDevice GetVkDevice() const { return m_device; }
				VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
				VkQueue GetPresentQueue() const { return m_presentQueue; }

			private:
				VkDevice m_device = VK_NULL_HANDLE;
				VkQueue m_graphicsQueue{};
				VkQueue m_presentQueue{};

			};
		}
	}
}

#endif