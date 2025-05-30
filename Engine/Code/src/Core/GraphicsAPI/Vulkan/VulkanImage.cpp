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
			void VulkanImage::Create(const RHI::ImageType a_type, RHI::ImageFormat a_format, const RHI::ImageData a_data,
			                         RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice,
			                         RHI::ICommandPool* a_commandPool)
			{
				VkPhysicalDevice t_physicalDevice = a_physicalDevice->CastVulkan()->GetVkPhysicalDevice();
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkCommandPool t_commandPool = a_commandPool->CastVulkan()->GetVkCommandPool();

				VkFormat t_format = static_cast<VkFormat>((int)a_format);
				if (a_type == RHI::ImageType::TEXTURE)
				{
					const VkDeviceSize t_imageSize = a_data.mWidth * a_data.mHeight * a_data.channels;
					VkBuffer t_stagingBuffer;
					VkDeviceMemory t_stagingBufferMemory;
					VulkanBuffer::CreateBuffer(t_imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, t_stagingBuffer, t_stagingBufferMemory, t_logicalDevice, t_physicalDevice);
					void* t_data;
					vkMapMemory(t_logicalDevice, t_stagingBufferMemory, 0, t_imageSize, 0, &t_data);
					memcpy(t_data, a_data.data, static_cast<size_t>(t_imageSize));
					vkUnmapMemory(t_logicalDevice, t_stagingBufferMemory);

					CreateSampler(&m_sampler, t_logicalDevice, t_physicalDevice);
					CreateImage(&m_image, &m_memory, t_logicalDevice, t_physicalDevice,a_data.mWidth, a_data.mHeight, t_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
					TransitionImageLayout(m_image, t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, t_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
					CopyBufferToImage(t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, t_stagingBuffer, m_image, a_data.mWidth, a_data.mHeight);
					TransitionImageLayout(m_image, t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, t_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

					vkDestroyBuffer(t_logicalDevice, t_stagingBuffer, nullptr);
					vkFreeMemory(t_logicalDevice, t_stagingBufferMemory, nullptr);

					CreateImageView(m_image, &m_view, t_logicalDevice, VK_IMAGE_VIEW_TYPE_2D ,t_format, VK_IMAGE_ASPECT_COLOR_BIT);

				}
				else if (a_type == RHI::ImageType::DEPTH)
				{
					const VkFormat t_depthFormat = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
																									VK_IMAGE_TILING_OPTIMAL,
																									VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

					CreateImage(&m_image, &m_memory,t_logicalDevice, t_physicalDevice, a_data.mWidth, a_data.mHeight, t_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
					CreateImageView(m_image, &m_view,t_logicalDevice, VK_IMAGE_VIEW_TYPE_2D, t_format, VK_IMAGE_ASPECT_DEPTH_BIT);
					TransitionImageLayout(m_image, a_logicalDevice->CastVulkan()->GetVkDevice(),
					                      a_logicalDevice->CastVulkan()->GetGraphicsQueue(),
					                      a_commandPool->CastVulkan()->GetVkCommandPool(), t_format,
					                      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
				}
				else if (a_type == RHI::ImageType::CUBEMAP)
				{
					// unique staging buffer holding all the cubemap texture (big chunk of memory)
					const VkDeviceSize t_imageSize = a_data.mWidth * a_data.mHeight * a_data.channels * 6;
					VkBuffer t_stagingBuffer;
					VkDeviceMemory t_stagingBufferMemory;
					VulkanBuffer::CreateBuffer(t_imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, t_stagingBuffer, t_stagingBufferMemory, t_logicalDevice, t_physicalDevice);


					void* t_data;
					vkMapMemory(t_logicalDevice, t_stagingBufferMemory, 0, t_imageSize, 0, &t_data);
					memcpy(t_data, a_data.data, static_cast<size_t>(t_imageSize));
					vkUnmapMemory(t_logicalDevice, t_stagingBufferMemory);

					// Do not need to change for cubemap
					CreateSampler(&m_sampler, t_logicalDevice, t_physicalDevice);

					// Has been change for cubemap
					CreateImage(&m_image, &m_memory, t_logicalDevice, t_physicalDevice, a_data.mWidth, a_data.mHeight, t_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 6, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);

					// Has been change for cubemap
					TransitionImageLayout(m_image, t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, t_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6);

					// Do not need to change for cubemap
					CopyBufferToImage(t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, t_stagingBuffer, m_image, a_data.mWidth, a_data.mHeight, 6);

					TransitionImageLayout(m_image, t_logicalDevice, a_logicalDevice->CastVulkan()->GetGraphicsQueue(), t_commandPool, t_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 6);

					vkDestroyBuffer(t_logicalDevice, t_stagingBuffer, nullptr);
					vkFreeMemory(t_logicalDevice, t_stagingBufferMemory, nullptr);
					
					CreateImageView(m_image, &m_view, t_logicalDevice, VK_IMAGE_VIEW_TYPE_CUBE,t_format, VK_IMAGE_ASPECT_COLOR_BIT, 6);
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
				vkDestroySampler(t_device, m_sampler, nullptr);
			}

			void VulkanImage::CreateImage(VkImage* a_image, VkDeviceMemory* a_memory, const VkDevice a_logicalDevice,
											const VkPhysicalDevice a_physicalDevice, const uint32_t a_width,
											const uint32_t a_height, const VkFormat a_format,
											const VkImageTiling a_tiling, const VkImageUsageFlags a_usage,
											const VkMemoryPropertyFlags a_properties, const int a_layers, const VkImageCreateFlags a_flags)
			{
				VkImageCreateInfo t_imageInfo{};
				t_imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				t_imageInfo.imageType = VK_IMAGE_TYPE_2D;
				t_imageInfo.extent.width = a_width;
				t_imageInfo.extent.height = a_height;
				t_imageInfo.extent.depth = 1;
				t_imageInfo.mipLevels = 1;
				t_imageInfo.arrayLayers = a_layers;  // Base = 1, 6 for cubemap
				t_imageInfo.format = a_format;
				t_imageInfo.tiling = a_tiling;
				t_imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				t_imageInfo.usage = a_usage;
				t_imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				t_imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				t_imageInfo.flags = a_flags; // only usefull for cubemap (VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)

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
			void VulkanImage::CreateSampler(VkSampler* a_sampler, VkDevice a_logicalDevice, VkPhysicalDevice a_physicalDevice)
			{
				VkPhysicalDeviceProperties properties{};
				vkGetPhysicalDeviceProperties(a_physicalDevice, &properties);

				VkSamplerCreateInfo samplerInfo{};
				samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter = VK_FILTER_LINEAR;
				samplerInfo.minFilter = VK_FILTER_LINEAR;
				samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.anisotropyEnable = VK_FALSE;
				samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
				samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				samplerInfo.unnormalizedCoordinates = VK_FALSE;
				samplerInfo.compareEnable = VK_FALSE;
				samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; // CUBEMAP

				VK_CHECK(vkCreateSampler(a_logicalDevice, &samplerInfo, nullptr, a_sampler), "Can't Create texture sampler");
			}
			void VulkanImage::CreateImageView(const VkImage a_image, VkImageView* a_view, const VkDevice a_logicalDevice, const VkImageViewType a_viewType, const VkFormat a_format, const VkImageAspectFlags a_aspectFlags, const int a_layerCount)
			{
				VkImageViewCreateInfo t_viewInfo{};
				t_viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				t_viewInfo.image = a_image;
				t_viewInfo.viewType = a_viewType;
				t_viewInfo.format = a_format;
				t_viewInfo.subresourceRange.aspectMask = a_aspectFlags;
				t_viewInfo.subresourceRange.baseMipLevel = 0;
				t_viewInfo.subresourceRange.levelCount = 1;
				t_viewInfo.subresourceRange.baseArrayLayer = 0;
				t_viewInfo.subresourceRange.layerCount = a_layerCount;

				VK_CHECK(vkCreateImageView(a_logicalDevice, &t_viewInfo, nullptr, a_view), "Failed to create image view");
			}

			auto VulkanImage::TransitionImageLayout(const VkImage a_image, const VkDevice a_logicalDevice,
			                                        const VkQueue a_queue, const VkCommandPool a_commandPool,
			                                        const VkFormat a_format, const VkImageLayout a_oldLayout,
			                                        const VkImageLayout a_newLayout, int a_layerCount) -> void
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
				t_barrier.subresourceRange.layerCount = a_layerCount; // Base = 1, 6 for cubemap

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
			                                    const uint32_t a_height, const int a_layerCount) -> void
			{
				const VkCommandBuffer t_commandBuffer = VulkanCommandPool::BeginSingleTimeCommands(a_logicalDevice, a_commandPool);

				std::vector<VkBufferImageCopy> t_regions(a_layerCount);

				for (int i = 0; i < a_layerCount; ++i) 
				{
					t_regions[i].bufferOffset = static_cast<VkDeviceSize>(a_width) * a_height * 4 * i;
					t_regions[i].bufferRowLength = 0;
					t_regions[i].bufferImageHeight = 0;
					t_regions[i].imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					t_regions[i].imageSubresource.mipLevel = 0;
					t_regions[i].imageSubresource.baseArrayLayer = i;
					t_regions[i].imageSubresource.layerCount = 1;
					t_regions[i].imageOffset = { 0, 0, 0 };
					t_regions[i].imageExtent = { a_width, a_height, 1 };
				}

				vkCmdCopyBufferToImage(t_commandBuffer, a_buffer, a_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					static_cast<uint32_t>(t_regions.size()), t_regions.data());

				/*VkBufferImageCopy t_region{};
				t_region.bufferOffset = 0;
				t_region.bufferRowLength = 0;
				t_region.bufferImageHeight = 0;
				t_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				t_region.imageSubresource.mipLevel = 0;
				t_region.imageSubresource.baseArrayLayer = 0;
				t_region.imageSubresource.layerCount = 1;
				t_region.imageOffset = { .x= 0, .y= 0, .z= 0};
				t_region.imageExtent = { .width= a_width,.height= a_height,.depth= 1};

				vkCmdCopyBufferToImage(t_commandBuffer, a_buffer, a_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,&t_region);*/

				VulkanCommandPool::EndSingleTimeCommands(t_commandBuffer, a_commandPool, a_logicalDevice, a_queue);
			}

			bool VulkanImage::HasStencilComponent(const VkFormat a_format)
			{
				return a_format == VK_FORMAT_D32_SFLOAT_S8_UINT || a_format == VK_FORMAT_D24_UNORM_S8_UINT;
			}

		}
	}
}