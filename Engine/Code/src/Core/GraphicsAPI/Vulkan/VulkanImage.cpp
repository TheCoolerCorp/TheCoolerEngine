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

					CreateImage(t_logicalDevice, t_physicalDevice, a_data.mWidth, a_data.mHeight, t_depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
					CreateImageView(t_logicalDevice, t_depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
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

			void VulkanImage::CreateImage(VkDevice a_device, VkPhysicalDevice a_physicalDevice, uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags properties, VkImageLayout a_layout)
			{

			}

			void VulkanImage::CreateImageView(VkDevice a_device, VkFormat a_format, VkImageAspectFlags a_aspectFlags)
			{
				 
			}
		}
	}
}