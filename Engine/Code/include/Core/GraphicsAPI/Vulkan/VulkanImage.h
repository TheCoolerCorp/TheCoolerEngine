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

			private:
				void CreateImage(VkDevice a_device, VkPhysicalDevice a_physicalDevice, uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags properties, VkImageLayout a_layoyt);
				void CreateImageView(VkDevice a_device, VkFormat a_format, VkImageAspectFlags a_aspectFlags);

				VkImage m_image = VK_NULL_HANDLE;
				VkImageView m_view = VK_NULL_HANDLE;
				VkDeviceMemory m_memory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif