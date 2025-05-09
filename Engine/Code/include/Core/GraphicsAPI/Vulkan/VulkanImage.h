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

				ENGINE_API void Create(RHI::ImageType a_type, RHI::ImageFormat a_format, RHI::ImageData a_data, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				[[nodiscard]] ENGINE_API VkImage GetImage() { return m_image; }
				[[nodiscard]] ENGINE_API VkImageView GetView() { return m_view; }
				[[nodiscard]] ENGINE_API VkDeviceMemory GetMemory() { return m_memory; }
				[[nodiscard]] ENGINE_API VkSampler GetSampler() { return m_sampler; }
		
				ENGINE_API static void CreateImage(VkImage* a_image, VkDeviceMemory* a_memory, const VkDevice a_logicalDevice,
					const VkPhysicalDevice a_physicalDevice, const uint32_t a_width,
					const uint32_t a_height, const VkFormat a_format,
					const VkImageTiling a_tiling, const VkImageUsageFlags a_usage,
					const VkMemoryPropertyFlags a_properties, const int a_layers = 1, const VkImageCreateFlags a_flags = {});
				// FINISH LATER
				ENGINE_API static void CreateSampler(VkSampler* a_sampler, VkDevice a_logicalDevice, VkPhysicalDevice a_physicalDevice);
				ENGINE_API static void CreateImageView(const VkImage a_image, VkImageView* a_view, const VkDevice a_logicalDevice, const VkImageViewType a_viewType, const VkFormat a_format, const VkImageAspectFlags a_aspectFlags, const int a_layerCount =1);
				ENGINE_API static void TransitionImageLayout(const VkImage a_image, const VkDevice a_logicalDevice,
					const VkQueue a_queue, const VkCommandPool a_commandPool,
					const VkFormat a_format, const VkImageLayout a_oldLayout,
					const VkImageLayout a_newLayout, int a_layerCount = 1);
				ENGINE_API static void CopyBufferToImage(VkDevice a_logicalDevice, VkQueue a_queue, VkCommandPool a_commandPool, VkBuffer a_buffer, VkImage a_image, uint32_t a_width, uint32_t a_height);

				ENGINE_API static bool HasStencilComponent(VkFormat a_format);

				 

			private:
				VkImage m_image = VK_NULL_HANDLE;
				VkImageView m_view = VK_NULL_HANDLE;
				VkDeviceMemory m_memory = VK_NULL_HANDLE;
				VkSampler m_sampler = VK_NULL_HANDLE;
			};
		}
	}
}

#endif