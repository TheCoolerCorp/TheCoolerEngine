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
			class ENGINE_API VulkanImage : public RHI::IImage
			{
			public:
				~VulkanImage() override = default;
				GraphicsAPI::VulkanImage* CastVulkan() override { return this; }

				void Create(RHI::ImageType a_type, RHI::ImageData a_data, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool) override;
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				VkImage GetImage() { return m_image; }
				VkImageView GetView() { return m_view; }
				VkDeviceMemory GetMemory() { return m_memory; }

		
				static void CreateImage(VkImage* a_image, VkDeviceMemory* a_memory, VkDevice a_logicalDevice, VkPhysicalDevice a_physicalDevice, uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags properties);
				static void CreateImageView(VkImage a_image, VkImageView* a_view, VkDevice a_logicalDevice, VkFormat a_format, VkImageAspectFlags a_aspectFlags);
				static void TransitionImageLayout(VkImage a_image, VkDevice a_logicalDevice, VkQueue a_queue, VkCommandPool a_commandPool, VkFormat a_format, VkImageLayout oldLayout, VkImageLayout newLayout);
				static void CopyBufferToImage(VkDevice a_logicalDevice, VkQueue a_queue, VkCommandPool a_commandPool, VkBuffer a_buffer, VkImage a_image, uint32_t a_width, uint32_t a_height);

				static bool HasStencilComponent(VkFormat format);

			private:
				VkImage m_image = VK_NULL_HANDLE;
				VkImageView m_view = VK_NULL_HANDLE;
				VkDeviceMemory m_memory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif