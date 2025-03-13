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
			class ENGINE_API VulkanBuffer : public RHI::IBuffer
			{
			public:
				void CreateVertexBuffer(std::vector<Ressources::Vertex> a_vertices) override;
				void CreateIndexBuffer(std::vector<uint32_t> a_indices) override;
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				VulkanBuffer* CastVulkan() override { return this; }

				static void CreateBuffer(VkDeviceSize a_size, VkBufferUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkBuffer& a_buffer, VkDeviceMemory& a_bufferMemory, VkDevice a_device, VkPhysicalDevice a_physicalDevice);

			private:
				static uint32_t FindMemoryType(uint32_t a_typeFilter, VkMemoryPropertyFlags a_properties, VkPhysicalDevice a_physicalDevice);

				VkBuffer m_buffer = VK_NULL_HANDLE;
				VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;

			};
		}
	}
}

#endif