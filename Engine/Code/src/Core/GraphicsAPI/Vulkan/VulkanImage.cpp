#include "Core/GraphicsAPI/Vulkan/VulkanImage.h"

#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanImage::Create(RHI::ImageType a_type, RHI::ImageData a_data, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool)
			{
				VkPhysicalDevice t_physicalDevice = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkCommandPool t_commandPool = a_commandPool->CastVulkan()->GetVkCommandPool();


				if (a_type == RHI::ImageType::TEXTURE)
				{
					//VkDeviceSize t_imageSize = a_data.mWidth * a_data.mHeight * 4;
					//VkBuffer stagingBuffer;
					//VkDeviceMemory stagingBufferMemory;
					//VulkanBuffer::VulkanCreateStagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vkDevice->device, vkDevice->physicalDevice);
					//void* data;
					//vkMapMemory(t_logicalDevice, stagingBufferMemory, 0, t_imageSize, 0, &data);
					//memcpy(data, a_data.data, static_cast<size_t>(t_imageSize));
					//vkUnmapMemory(t_logicalDevice, stagingBufferMemory);


					//CreateImage(t_logicalDevice, vkDevice->physicalDevice, t_textureWidth, t_textureHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
					//TransitionImageLayout(vkDevice->device, vkDevice->graphicsQueue, vkCommandPool->commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
					//CopyBufferToImage(vkDevice->device, vkDevice->graphicsQueue, vkCommandPool->commandPool, stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
					//TransitionImageLayout(vkDevice->device, vkDevice->graphicsQueue, vkCommandPool->commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

					//vkDestroyBuffer(vkDevice->device, stagingBuffer, nullptr);
					//vkFreeMemory(vkDevice->device, stagingBufferMemory, nullptr);

					//VulkanCreateImageView(vkDevice->device, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
				}
				else if (a_type == RHI::ImageType::DEPTH)
				{
					VkFormat t_depthFormat = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
																									VK_IMAGE_TILING_OPTIMAL,
																									VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

					CreateImage(&m_image, &m_memory,t_logicalDevice, t_physicalDevice, a_data.mWidth, a_data.mHeight, t_depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
					CreateImageView(&m_image, &m_view,t_logicalDevice, t_depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
				}
				else
				{
					// Not define
				}
			}

			void VulkanImage::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				vkDestroyImageView(t_device, m_view, nullptr);
				vkDestroyImage(t_device, m_image, nullptr);
				vkFreeMemory(t_device, m_memory, nullptr);
			}

			void VulkanImage::CreateImage(VkImage* a_image, VkDeviceMemory* a_memory, VkDevice a_logicalDevice, VkPhysicalDevice a_physicalDevice, uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags properties, VkImageLayout a_layout)
			{
				VkImageCreateInfo imageInfo{};
				imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				imageInfo.imageType = VK_IMAGE_TYPE_2D;
				imageInfo.extent.width = a_width;
				imageInfo.extent.height = a_height;
				imageInfo.extent.depth = 1;
				imageInfo.mipLevels = 1;
				imageInfo.arrayLayers = 1;
				imageInfo.format = a_format;
				imageInfo.tiling = a_tiling;
				imageInfo.initialLayout = a_layout;
				imageInfo.usage = a_usage;
				imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				VK_CHECK(vkCreateImage(a_logicalDevice, &imageInfo, nullptr, a_image), "Failed to create image!");

				VkMemoryRequirements memRequirements;
				vkGetImageMemoryRequirements(a_logicalDevice, *a_image, &memRequirements);

				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;

				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(a_physicalDevice, &memProperties);
				for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
				{
					if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
					{
						allocInfo.memoryTypeIndex = i;
					}
				}

				VK_CHECK(vkAllocateMemory(a_logicalDevice, &allocInfo, nullptr, a_memory), "Failed to allocate memory for image");

				vkBindImageMemory(a_logicalDevice, *a_image, *a_memory, 0);
			}

			void VulkanImage::CreateImageView(VkImage* a_image, VkImageView* a_view, VkDevice a_logicalDevice, VkFormat a_format, VkImageAspectFlags a_aspectFlags)
			{
				VkImageViewCreateInfo viewInfo{};
				viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image = *a_image;
				viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				viewInfo.format = a_format;
				viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.subresourceRange.baseMipLevel = 0;
				viewInfo.subresourceRange.levelCount = 1;
				viewInfo.subresourceRange.baseArrayLayer = 0;
				viewInfo.subresourceRange.layerCount = 1;
				viewInfo.subresourceRange.aspectMask = a_aspectFlags;

				VK_CHECK(vkCreateImageView(a_logicalDevice, &viewInfo, nullptr, a_view), "Failed to create image view");
			}
		}
	}
}