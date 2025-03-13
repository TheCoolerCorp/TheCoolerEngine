#ifndef VULKANIMAGE_H
#define VULKANIMAGE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

#include "Core/Interfaces/IImage.h"

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

				void Create(int a_width, int a_height, RHI::ImageType a_type, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool) override;
				void Destroy() override;

				VkImage GetImage() { return m_image; }
				VkImageView GetView() { return m_view; }
				VkDeviceMemory GetMemory() { return m_memory; }

			private:
				void CreateImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
				void CreateImageView(VkDevice device, VkFormat format, VkImageAspectFlags aspectFlags);

				VkImage m_image = VK_NULL_HANDLE;
				VkImageView m_view = VK_NULL_HANDLE;
				VkDeviceMemory m_memory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif