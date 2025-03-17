#ifndef VULKANIMAGE_H
#define VULKANIMAGE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

#include "Core/Interfaces/IImage.h"

#include <iostream>
#include <cstdarg>

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanImage : public RHI::IImage
			{
			public:
				ENGINE_API ~VulkanImage() override = default;
				ENGINE_API VulkanImage* CastVulkan() override { return this; }

				ENGINE_API void Create(RHI::ImageType a_type, RHI::ImageData a_data, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API VkImage GetImage() { return m_image; }
				ENGINE_API VkImageView GetView() { return m_view; }
				ENGINE_API VkDeviceMemory GetMemory() { return m_memory; }

		
				ENGINE_API static void CreateImage(VkImage* a_image, VkDeviceMemory* a_memory, VkDevice a_logicalDevice, VkPhysicalDevice a_physicalDevice, uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags properties);
				ENGINE_API static void CreateImageView(VkImage a_image, VkImageView* a_view, VkDevice a_logicalDevice, VkFormat a_format, VkImageAspectFlags a_aspectFlags);
				ENGINE_API static void TransitionImageLayout(VkImage a_image, VkDevice a_logicalDevice, VkQueue a_queue, VkCommandPool a_commandPool, VkFormat a_format, VkImageLayout oldLayout, VkImageLayout newLayout);
				ENGINE_API static void CopyBufferToImage(VkDevice a_logicalDevice, VkQueue a_queue, VkCommandPool a_commandPool, VkBuffer a_buffer, VkImage a_image, uint32_t a_width, uint32_t a_height);

				ENGINE_API static bool HasStencilComponent(VkFormat format);

			private:
				VkImage m_image = VK_NULL_HANDLE;
				VkImageView m_view = VK_NULL_HANDLE;
				VkDeviceMemory m_memory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif