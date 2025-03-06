#ifndef QUEUEFAMILIES_H
#define QUEUEFAMILIES_H

#include <vulkan/vulkan.h>
#include <optional>
#include <set>

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			struct QueueFamilyIndices {
				bool IsComplete() const;
				static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_physicalDevice);
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