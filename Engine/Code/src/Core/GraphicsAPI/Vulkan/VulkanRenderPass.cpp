#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"

#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/Renderer/Renderer.h"


namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			std::vector<RenderPassFlags> VulkanRenderPassManager::m_renderPassFlags = std::vector<RenderPassFlags>();

			void VulkanRenderPassManager::Create(Renderer* renderer)
			{
				m_renderer = renderer;
				if (!HasFlag(FLAG_VK_RHI_OVERRIDE_DEFAULT_RENDERPASS))
				{
					CreateDefaultRenderPass(renderer);
				}
			}


			void VulkanRenderPassManager::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				m_sceneRenderPass->Destroy();
				delete m_sceneRenderPass;
			}

			void VulkanRenderPassManager::CreateDefaultRenderPass(Renderer* a_renderer)
			{
				VkFormat imageFormat = a_renderer->GetSwapChain()->CastVulkan()->GetImageFormat();
				VkExtent2D extent = a_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
				VkDevice device = a_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

				RenderPassConfig config{};

				// Color attachment setup
				RenderPassAttachment colorAttachment{};
				colorAttachment.format = imageFormat;
				colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				colorAttachment.isDepth = false;

				// Depth attachment setup
				RenderPassAttachment depthAttachment{};
				depthAttachment.format = a_renderer->GetPhysicalDevice()->CastVulkan()->FindSupportedFormat(
					{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
				);
				depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				depthAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				depthAttachment.isDepth = true;

				// Add attachments to config
				config.attachments.push_back(colorAttachment); // index 0
				config.attachments.push_back(depthAttachment); // index 1

				// Subpass setup
				SubpassConfig subpass{};
				subpass.colorAttachmentIndices = { 0 };
				subpass.depthAttachmentIndex = 1;

				config.subpasses.push_back(subpass);

				// Dependency
				VkSubpassDependency dependency{};
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				dependency.srcAccessMask = 0;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				config.dependencies.push_back(dependency);

				// Extent
				config.extent = extent;
				config.setViewportAndScissor = true;
				config.useSwapChainFramebuffers = true;

				m_sceneRenderPass = new VulkanRenderPass(device, a_renderer);
				
				m_sceneRenderPass->Create(config);
				//m_sceneRenderPass->SetFramebuffers(m_renderer->GetSwapChain()->CastVulkan()->GetFramebuffers());

				m_sceneRenderPass->SetDrawFunc(
					[this](RecordRenderPassinfo a_info, const std::vector<Core::RHI::IRenderObject*>& a_renderObjects,
						const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers,
						const std::vector<Core::RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices)
					{
						const VkPipelineLayout t_layout = a_info.graphicPipeline->GetLayout();

						const VkDescriptorSet t_cameraDescriptorSet = a_info.camera->GetDescriptor()->CastVulkan()->GetDescriptorSet();
						vkCmdBindDescriptorSets(a_info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 0, 1, &t_cameraDescriptorSet, 0, nullptr);

						for (int i = 0; i < a_renderObjects.size(); ++i)
						{
							if (a_vertexBuffers.at(i)->CastVulkan()->GetBuffer() != nullptr)
							{
								VkBuffer t_vertexBuffer = a_vertexBuffers.at(i)->CastVulkan()->GetBuffer();
								constexpr VkDeviceSize t_offsets[] = { 0 };
								vkCmdBindVertexBuffers(a_info.commandBuffer, 0, 1, &t_vertexBuffer, t_offsets);
							}

							if (a_indexBuffers.at(i)->CastVulkan()->GetBuffer() != nullptr)
							{
								vkCmdBindIndexBuffer(a_info.commandBuffer, a_indexBuffers.at(i)->CastVulkan()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
							}

							vkCmdBindDescriptorSets(a_info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 1, 1, &a_renderObjects.at(i)->CastVulkan()->GetDescriptorSets()[a_info.imageIndex], 0, nullptr);

							vkCmdDrawIndexed(a_info.commandBuffer, a_nbIndices.at(i), 1, 0, 0, 0);
						}
					}
				);
			}

			void VulkanRenderPassManager::RunSceneRenderPass(RecordRenderPassinfo a_info,
				const std::vector<Core::RHI::IRenderObject*>& a_renderObjects,
				const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers,
				const std::vector<Core::RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices)
			{
				m_sceneRenderPass->RecordRenderPass(a_info, a_renderObjects, a_vertexBuffers, a_indexBuffers, a_nbIndices);
			}

			VulkanRenderPass::VulkanRenderPass(VkDevice device, Renderer* renderer)
			{
				m_device = device;
				m_renderer = renderer;
			}

			VulkanRenderPass::~VulkanRenderPass()
			{
				Destroy();
			}

			void VulkanRenderPass::Create(const RenderPassConfig& config)
			{
				m_config = config;

				std::vector<VkAttachmentDescription> vkAttachments;
                vkAttachments.reserve(config.attachments.size());

                for (const auto& attachment : config.attachments)
                {
					VkAttachmentDescription desc{};
					desc.format = attachment.format;
					desc.samples = VK_SAMPLE_COUNT_1_BIT;
					desc.loadOp = attachment.loadOp;
					desc.storeOp = attachment.storeOp;
					desc.stencilLoadOp = attachment.isDepth ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					desc.initialLayout = attachment.initialLayout;
					desc.finalLayout = attachment.finalLayout;

                    vkAttachments.push_back(desc);
                }

				//prepare subpass descriptions
				std::vector<VkSubpassDescription> vkSubpasses;
				std::vector<std::vector<VkAttachmentReference>> colorAttachmentRefs;
				std::vector<VkAttachmentReference> depthAttachmentRefs;

                for (const auto& subpass : config.subpasses)
                {
					std::vector<VkAttachmentReference> colorRefs;
                    for (uint32_t idx : subpass.colorAttachmentIndices)
                    {
						VkAttachmentReference ref{};
						ref.attachment = idx;
						ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						colorRefs.push_back(ref);
                    }

					VkAttachmentReference depthRef{};
                    if (subpass.depthAttachmentIndex >= 0)
                    {
						depthRef.attachment = subpass.depthAttachmentIndex;
						depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    }

					colorAttachmentRefs.push_back(colorRefs);

					VkSubpassDescription subpassDesc{};
					subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
					subpassDesc.colorAttachmentCount = static_cast<uint32_t>(colorRefs.size());
					subpassDesc.pColorAttachments = colorAttachmentRefs.back().data();
					subpassDesc.pDepthStencilAttachment = subpass.depthAttachmentIndex >= 0 ? &depthRef : nullptr;

					if (subpass.depthAttachmentIndex >= 0)
						depthAttachmentRefs.push_back(depthRef);

					vkSubpasses.push_back(subpassDesc);
                }

				VkRenderPassCreateInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<uint32_t>(vkAttachments.size());
				renderPassInfo.pAttachments = vkAttachments.data();
				renderPassInfo.subpassCount = static_cast<uint32_t>(vkSubpasses.size());
				renderPassInfo.pSubpasses = vkSubpasses.data();
				renderPassInfo.dependencyCount = static_cast<uint32_t>(config.dependencies.size());
				renderPassInfo.pDependencies = config.dependencies.data();

				VK_CHECK(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass), "Failed to create render pass!");
			}

			void VulkanRenderPass::CreateFramebuffers(const std::vector<std::vector<VkImageView>>& a_views)
			{
				m_framebuffers.resize(a_views.size());
				for (size_t i = 0; i < a_views.size(); ++i)
				{
					VkFramebufferCreateInfo framebufferInfo{};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = m_renderPass;
					framebufferInfo.attachmentCount = static_cast<uint32_t>(a_views[i].size());
					framebufferInfo.pAttachments = a_views[i].data();
					framebufferInfo.width = m_config.extent.width;
					framebufferInfo.height = m_config.extent.height;
					framebufferInfo.layers = 1;
					VK_CHECK(vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_framebuffers[i]), "Failed to create framebuffer!");
				}
			}

			void VulkanRenderPass::SetFramebuffers(const std::vector<VkFramebuffer>& a_buffers)
			{
				m_framebuffers = a_buffers;
			}

			void VulkanRenderPass::CreateAttachments()
			{

				m_AttachmentResources.resize(m_config.attachments.size());

				for (size_t i = 0; i < m_config.attachments.size(); ++i)
				{
					const auto& info = m_config.attachments[i];

					VkImageCreateInfo imageInfo = {};
					imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
					imageInfo.imageType = VK_IMAGE_TYPE_2D;
					imageInfo.format = info.format;
					imageInfo.extent.width = m_config.extent.width;
					imageInfo.extent.height = m_config.extent.height;
					imageInfo.extent.depth = 1;
					imageInfo.mipLevels = 1;
					imageInfo.arrayLayers = 1;
					imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
					imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
					imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

					if (info.isDepth)
						imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

					imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
					imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

					// Create image
					if (vkCreateImage(m_device, &imageInfo, nullptr, &m_AttachmentResources[i].image) != VK_SUCCESS)
					{
						throw std::runtime_error("Failed to create attachment image");
					}

					// Allocate memory
					VkMemoryRequirements memRequirements;
					vkGetImageMemoryRequirements(m_device, m_AttachmentResources[i].image, &memRequirements);

					VkMemoryAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = VulkanBuffer::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice()); 

					if (vkAllocateMemory(m_device, &allocInfo, nullptr, &m_AttachmentResources[i].memory) != VK_SUCCESS)
					{
						throw std::runtime_error("Failed to allocate image memory");
					}

					vkBindImageMemory(m_device, m_AttachmentResources[i].image, m_AttachmentResources[i].memory, 0);

					// Create image view
					VkImageViewCreateInfo viewInfo = {};
					viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					viewInfo.image = m_AttachmentResources[i].image;
					viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
					viewInfo.format = info.format;
					viewInfo.subresourceRange.aspectMask = info.aspectMask;
					viewInfo.subresourceRange.baseMipLevel = 0;
					viewInfo.subresourceRange.levelCount = 1;
					viewInfo.subresourceRange.baseArrayLayer = 0;
					viewInfo.subresourceRange.layerCount = 1;

					if (vkCreateImageView(m_device, &viewInfo, nullptr, &m_AttachmentResources[i].view) != VK_SUCCESS)
					{
						throw std::runtime_error("Failed to create image view");
					}
				}

			}

			void VulkanRenderPass::TransitionImageLayout(VkImageLayout newImageLayout)
			{
				VkDevice a_logicalDevice = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
				VkQueue a_queue = m_renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue();
				VkCommandPool a_commandPool = m_renderer->GetCommandPool()->CastVulkan()->GetVkCommandPool();
				for (auto& resource : m_AttachmentResources)
				{

					VulkanImage::TransitionImageLayout(
						resource.image,
						a_logicalDevice,
						a_queue,
						a_commandPool,
						VK_FORMAT_B8G8R8A8_SRGB,
						VK_IMAGE_LAYOUT_UNDEFINED,
						newImageLayout);
				}
			}

			void VulkanRenderPass::RecordRenderPass(RecordRenderPassinfo a_info,
				const std::vector<Core::RHI::IRenderObject*>& a_renderObjects,
				const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers,
				const std::vector<Core::RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices)
			{
				VkCommandBufferBeginInfo t_beginInfo{};
				t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

				VK_CHECK(vkBeginCommandBuffer(a_info.commandBuffer, &t_beginInfo), "failed to begin recording command buffer!");
				Begin(a_info.commandBuffer, a_info.imageIndex);
				//Check if the function has been set before calling it
				if (m_drawFunc == nullptr)
					LOG_ERROR("Tried to record render pass with no draw function set!");
				else
					m_drawFunc(a_info, a_renderObjects, a_vertexBuffers, a_indexBuffers, a_nbIndices);
				End(a_info.commandBuffer);
				VK_CHECK(vkEndCommandBuffer(a_info.commandBuffer), "failed to end command buffer!");
			}

			void VulkanRenderPass::Begin(VkCommandBuffer cmd, uint32_t imageIndex, VkSubpassContents contents)
			{
				const VkExtent2D t_swapChainExtent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
				std::vector<VkClearValue> clearValues;
				clearValues.resize(m_config.attachments.size());

				for (size_t i = 0; i < m_config.attachments.size(); ++i)
				{
					const auto& info = m_config.attachments[i];

					if (info.aspectMask & VK_IMAGE_ASPECT_COLOR_BIT)
					{
						clearValues[i].color = { {0.467f, 0.71f, 1.f, 0.996f} };
					}
					else if (info.aspectMask & VK_IMAGE_ASPECT_DEPTH_BIT)
					{
						clearValues[i].depthStencil = { .depth = 1.0f, .stencil = 0 };
					}
				}

				VkRenderPassBeginInfo beginInfo{};
				beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				beginInfo.renderPass = m_renderPass;
				if (m_config.useSwapChainFramebuffers)
					beginInfo.framebuffer = m_renderer->GetSwapChain()->CastVulkan()->GetFramebuffers()[imageIndex];
				else
					beginInfo.framebuffer = m_framebuffers[imageIndex];
				beginInfo.renderArea.offset = { 0, 0 };
				beginInfo.renderArea.extent = t_swapChainExtent;
				beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
				beginInfo.pClearValues = clearValues.data();

				vkCmdBeginRenderPass(cmd, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//TEMPORARY: MAKE RENDERPASS STORE REFERENCE TO ITS ASSOCIATED PIPELINE LATER
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_renderer->GetPipeline()->CastVulkan()->GetPipeline());

				//set viewport and scissor if configured to do so
				if (m_config.setViewportAndScissor)
				{
					VkViewport viewport{};
					viewport.x = 0.0f;
					viewport.y = 0.0f;
					viewport.width = static_cast<float>(t_swapChainExtent.width);
					viewport.height = static_cast<float>(t_swapChainExtent.height);
					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;
					vkCmdSetViewport(cmd, 0, 1, &viewport);

					VkRect2D scissor{};
					scissor.offset = { 0, 0 };
					scissor.extent = t_swapChainExtent;
					vkCmdSetScissor(cmd, 0, 1, &scissor);
				}
			}

			void VulkanRenderPass::End(VkCommandBuffer cmd)
			{
				vkCmdEndRenderPass(cmd);
			}

			void VulkanRenderPass::Destroy()
			{
				if (m_renderPass != VK_NULL_HANDLE)
				{
					vkDestroyRenderPass(m_device, m_renderPass, nullptr);
					m_renderPass = VK_NULL_HANDLE;
				}

				for (auto& res : m_AttachmentResources)
				{
					if (res.view != VK_NULL_HANDLE) {
						vkDestroyImageView(m_device, res.view, nullptr);
					}
					if (res.image != VK_NULL_HANDLE) {
						vkDestroyImage(m_device, res.image, nullptr);
					}
					if (res.memory != VK_NULL_HANDLE) {
						vkFreeMemory(m_device, res.memory, nullptr);
					}
				}
				m_AttachmentResources.clear();
			}
		}
	}
}
