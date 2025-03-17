#ifndef VULKANBUFFER_H
#define VULKANBUFFER_H

#include <vulkan/vulkan.h>

#include "Core/Interfaces/IBuffer.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanCommandPool;
			class VulkanLogicalDevice;

			class VulkanBuffer : public RHI::IBuffer
			{
			public:
				ENGINE_API void Create(RHI::BufferType a_type, RHI::BufferData a_data, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API VulkanBuffer* CastVulkan() override { return this; }

				ENGINE_API static void CreateBuffer(VkDeviceSize a_size, VkBufferUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkBuffer& a_buffer, VkDeviceMemory& a_bufferMemory, VkDevice a_device, VkPhysicalDevice a_physicalDevice);
				ENGINE_API static void CopyBuffer(const VulkanCommandPool* a_commandPool, VkBuffer a_srcBuffer, VkBuffer a_dstBuffer, VkDeviceSize a_size, const VulkanLogicalDevice* a_logicalDevice);

				ENGINE_API VkBuffer GetBuffer() const { return m_buffer; }

			private:
				ENGINE_API static uint32_t FindMemoryType(uint32_t a_typeFilter, VkMemoryPropertyFlags a_properties, VkPhysicalDevice a_physicalDevice);

				VkBuffer m_buffer = VK_NULL_HANDLE;
				VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;

			};
		}
	}
}

#endif