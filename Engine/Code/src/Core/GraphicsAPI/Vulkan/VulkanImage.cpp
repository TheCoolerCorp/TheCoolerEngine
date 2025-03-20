#include "Core/GraphicsAPI/Vulkan/VulkanImage.h"

#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanImage::Create(const RHI::ImageType a_type, const RHI::ImageData a_data,
			                         RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice,
			                         RHI::ICommandPool* a_commandPool)
			{
				VkPhysicalDevice t_physicalDevice = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkCommandPool t_commandPool = a_commandPool->CastVulkan()->GetVkCommandPool();


				if (a_type == RHI::ImageType::TEXTURE)
				{
					const VkDeviceSize t_imageSize = a_data.mWidth * a_data.mHeight * 4;
					VkBuffer t_stagingBuffer;
					VkDeviceMemory t_stagingBufferMemory;
					VulkanBuffer::CreateBuffer(t_imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, t_stagingBuffer, t_stagingBufferMemory, t_logicalDevice, t_physicalDevice);
					void* t_data;
					vkMapMemory(t_logicalDevice, t_stagingBufferMemory, 0, t_imageSize, 0, &t_data);
					memcpy(t_data, a_data.data, static_cast<size_t>(t_imageSize));
					vkUnmapMemory(t_logicalDevice, t_stagingBufferMemory);

					CreateImage(&m_image, &m_memory, t_logicalDevice, t_physicalDevice, a_data.mWidth, a_data.mHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
					TransitionImageLayout(m_image, t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
					CopyBufferToImage(t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, t_stagingBuffer, m_image, a_data.mWidth, a_data.mHeight);
					TransitionImageLayout(m_image, t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

					vkDestroyBuffer(t_logicalDevice, t_stagingBuffer, nullptr);
					vkFreeMemory(t_logicalDevice, t_stagingBufferMemory, nullptr);

					CreateImageView(m_image, &m_view, t_logicalDevice, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

				}
				else if (a_type == RHI::ImageType::DEPTH)
				{
					const VkFormat t_depthFormat = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
																									VK_IMAGE_TILING_OPTIMAL,
																									VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

					CreateImage(&m_image, &m_memory,t_logicalDevice, t_physicalDevice, a_data.mWidth, a_data.mHeight, t_depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
					CreateImageView(m_image, &m_view,t_logicalDevice, t_depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
					TransitionImageLayout(m_image, a_logicalDevice->CastVulkan()->GetVkDevice(),
					                      a_logicalDevice->CastVulkan()->GetGraphicsQueue(),
					                      a_commandPool->CastVulkan()->GetVkCommandPool(), t_depthFormat,
					                      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
				}
				else
				{
					LOG_ERROR("Image type undefined!");
				}
			}

			void VulkanImage::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				VkDevice t_device = a_logicalDevice->CastVulkan()->GetVkDevice();

				vkDestroyImageView(t_device, m_view, nullptr);
				vkDestroyImage(t_device, m_image, nullptr);
				vkFreeMemory(t_device, m_memory, nullptr);
			}

			void VulkanImage::CreateImage(VkImage* a_image, VkDeviceMemory* a_memory, const VkDevice a_logicalDevice,
			                              const VkPhysicalDevice a_physicalDevice, const uint32_t a_width,
			                              const uint32_t a_height, const VkFormat a_format,
			                              const VkImageTiling a_tiling, const VkImageUsageFlags a_usage,
			                              const VkMemoryPropertyFlags a_properties)
			{
				VkImageCreateInfo t_imageInfo{};
				t_imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				t_imageInfo.imageType = VK_IMAGE_TYPE_2D;
				t_imageInfo.extent.width = a_width;
				t_imageInfo.extent.height = a_height;
				t_imageInfo.extent.depth = 1;
				t_imageInfo.mipLevels = 1;
				t_imageInfo.arrayLayers = 1;
				t_imageInfo.format = a_format;
				t_imageInfo.tiling = a_tiling;
				t_imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				t_imageInfo.usage = a_usage;
				t_imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				t_imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				VK_CHECK(vkCreateImage(a_logicalDevice, &t_imageInfo, nullptr, a_image), "Failed to create image!");

				VkMemoryRequirements t_memRequirements;
				vkGetImageMemoryRequirements(a_logicalDevice, *a_image, &t_memRequirements);

				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = t_memRequirements.size;

				VkPhysicalDeviceMemoryProperties t_memProperties;
				vkGetPhysicalDeviceMemoryProperties(a_physicalDevice, &t_memProperties);
				for (uint32_t i = 0; i < t_memProperties.memoryTypeCount; i++) 
				{
					if ((t_memRequirements.memoryTypeBits & (1 << i)) && (t_memProperties.memoryTypes[i].propertyFlags & a_properties) == a_properties)
					{
						allocInfo.memoryTypeIndex = i;
					}
				}

				VK_CHECK(vkAllocateMemory(a_logicalDevice, &allocInfo, nullptr, a_memory), "Failed to allocate memory for image");

				vkBindImageMemory(a_logicalDevice, *a_image, *a_memory, 0);
			}

			void VulkanImage::CreateImageView(const VkImage a_image, VkImageView* a_view, const VkDevice a_logicalDevice, const VkFormat a_format, const VkImageAspectFlags a_aspectFlags)
			{
				VkImageViewCreateInfo t_viewInfo{};
				t_viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				t_viewInfo.image = a_image;
				t_viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				t_viewInfo.format = a_format;
				t_viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				t_viewInfo.subresourceRange.baseMipLevel = 0;
				t_viewInfo.subresourceRange.levelCount = 1;
				t_viewInfo.subresourceRange.baseArrayLayer = 0;
				t_viewInfo.subresourceRange.layerCount = 1;
				t_viewInfo.subresourceRange.aspectMask = a_aspectFlags;

				VK_CHECK(vkCreateImageView(a_logicalDevice, &t_viewInfo, nullptr, a_view), "Failed to create image view");
			}

			auto VulkanImage::TransitionImageLayout(const VkImage a_image, const VkDevice a_logicalDevice,
			                                        const VkQueue a_queue, const VkCommandPool a_commandPool,
			                                        const VkFormat a_format, const VkImageLayout a_oldLayout,
			                                        const VkImageLayout a_newLayout) -> void
			{
				const VkCommandBuffer t_commandBuffer = VulkanCommandPool::BeginSingleTimeCommands(a_logicalDevice, a_commandPool);

				VkImageMemoryBarrier t_barrier{};
				t_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				t_barrier.oldLayout = a_oldLayout;
				t_barrier.newLayout = a_newLayout;
				t_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				t_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				t_barrier.image = a_image;
				t_barrier.subresourceRange.baseMipLevel = 0;
				t_barrier.subresourceRange.levelCount = 1;
				t_barrier.subresourceRange.baseArrayLayer = 0;
				t_barrier.subresourceRange.layerCount = 1;

				VkPipelineStageFlags t_sourceStage = {};
				VkPipelineStageFlags t_destinationStage = {};


				if (a_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && a_newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
					t_barrier.srcAccessMask = 0;
					t_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

					t_sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
					t_destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				}
				else if (a_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && a_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
					t_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					t_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					t_sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					t_destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				}
				else if (a_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && a_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
					t_barrier.srcAccessMask = 0;
					t_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

					t_sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
					t_destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				}
				else 
				{
					LOG_ERROR("unsupported layout transition!");
				}

				if (a_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
				{
					t_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

					if (HasStencilComponent(a_format)) 
					{
						t_barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
					}
				}
				else 
				{
					t_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				}

				vkCmdPipelineBarrier(t_commandBuffer, t_sourceStage, t_destinationStage, 0, 0, nullptr, 0, nullptr, 1, &t_barrier);

				VulkanCommandPool::EndSingleTimeCommands(t_commandBuffer, a_commandPool, a_logicalDevice, a_queue);
			}

			auto VulkanImage::CopyBufferToImage(const VkDevice a_logicalDevice, const VkQueue a_queue, const VkCommandPool a_commandPool,
			                                    const VkBuffer a_buffer, const VkImage a_image, const uint32_t a_width,
			                                    const uint32_t a_height) -> void
			{
				const VkCommandBuffer t_commandBuffer = VulkanCommandPool::BeginSingleTimeCommands(a_logicalDevice, a_commandPool);

				VkBufferImageCopy t_region{};
				t_region.bufferOffset = 0;
				t_region.bufferRowLength = 0;
				t_region.bufferImageHeight = 0;
				t_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				t_region.imageSubresource.mipLevel = 0;
				t_region.imageSubresource.baseArrayLayer = 0;
				t_region.imageSubresource.layerCount = 1;
				t_region.imageOffset = { .x= 0, .y= 0, .z= 0};
				t_region.imageExtent = { .width= a_width,.height= a_height,.depth= 1};

				vkCmdCopyBufferToImage(t_commandBuffer, a_buffer, a_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,&t_region);

				VulkanCommandPool::EndSingleTimeCommands(t_commandBuffer, a_commandPool, a_logicalDevice, a_queue);
			}

			bool VulkanImage::HasStencilComponent(const VkFormat a_format)
			{
				return a_format == VK_FORMAT_D32_SFLOAT_S8_UINT || a_format == VK_FORMAT_D24_UNORM_S8_UINT;
			}

		}
	}
}