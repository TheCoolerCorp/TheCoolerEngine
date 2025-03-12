#ifndef VULKANCOMMANDPOOL_H
#define VULKANCOMMANDPOOL_H

#include <vulkan/vulkan.h>

#include "Core/Interfaces/ICommandPool.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanCommandPool : public RHI::ICommandPool
			{
			public:
				void Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ILogicalDevice* a_logicalDevice) override;
				VkCommandPool GetVkCommandPool() const { return m_commandPool; }
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

			private:
				VkCommandPool m_commandPool = VK_NULL_HANDLE;
			};
		}
	}
}

#endif