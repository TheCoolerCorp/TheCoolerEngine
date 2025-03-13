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
			void VulkanImage::Create(int a_width, int a_height, RHI::ImageType a_type, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice, RHI::ICommandPool* a_commandPool)
			{
				VkPhysicalDevice t_physicalDevice = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkCommandPool t_commandPool;

				VkFormat t_format;

				if (a_type == RHI::ImageType::TEXTURE)
				{

				}
				else if (a_type == RHI::ImageType::DEPTH)
				{
					//CreateImage(t_logicalDevice, t_physicalDevice, a_width, a_height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
					//CreateImageView(t_logicalDevice, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
				}
				else
				{

				}
			}

			void VulkanImage::Destroy()
			{
				
			}

			void VulkanImage::CreateImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
			{

			}

			void VulkanImage::CreateImageView(VkDevice device, VkFormat format, VkImageAspectFlags aspectFlags)
			{
				 
			}
		}
	}
}