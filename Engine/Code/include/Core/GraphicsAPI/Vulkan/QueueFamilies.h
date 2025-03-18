#ifndef QUEUEFAMILIES_H
#define QUEUEFAMILIES_H

#include <vulkan/vulkan.h>
#include <optional>
#include <set>

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			struct QueueFamilyIndices {
				ENGINE_API bool IsComplete() const;
				ENGINE_API static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_physicalDevice, VkSurfaceKHR a_surface);
				std::optional<uint32_t> GetGraphicsFamily() const { return m_graphicsFamily; }
				std::optional<uint32_t> GetPresentFamily() const { return m_presentFamily; }

			private:
				std::optional<uint32_t> m_graphicsFamily;
				std::optional<uint32_t> m_presentFamily;
			};
		}
	}
}

#endif