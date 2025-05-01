#include <utility>

#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"

#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/Renderer/Renderer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/Interfaces/IObjectDescriptor.h"
#include "Core/Interfaces/IBuffer.h"


namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			std::vector<RenderPassFlags> VulkanRenderPassManager::m_renderPassFlags = std::vector<RenderPassFlags>();

			void VulkanRenderPassManager::Create(Renderer* a_renderer)
			{
				m_renderer = a_renderer;
				if (!HasFlag(FLAG_VK_RHI_OVERRIDE_DEFAULT_RENDERPASS))
				{
					CreateDefaultRenderPass(a_renderer);
				}
			}


			void VulkanRenderPassManager::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				//m_sceneRenderPass->Destroy();
				delete m_sceneRenderPass;
				for (VulkanRenderPass* renderPass : m_renderPasses)
				{
					//renderPass->Destroy();
					delete renderPass;
				}
			}

			void VulkanRenderPassManager::CreateDefaultRenderPass(Renderer* a_renderer)
			{
				VkFormat t_imageFormat = a_renderer->GetSwapChain()->CastVulkan()->GetImageFormat();
				VkExtent2D t_extent = a_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
				VkDevice t_device = a_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

				RenderPassConfig config{};

				// Color attachment setup
				RenderPassAttachment t_colorAttachment;
				t_colorAttachment.format = t_imageFormat;
				t_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				t_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				t_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				t_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				t_colorAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				t_colorAttachment.isDepth = false;

				// Depth attachment setup
				RenderPassAttachment t_depthAttachment;
				t_depthAttachment.format = a_renderer->GetPhysicalDevice()->CastVulkan()->FindSupportedFormat(
					{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
				);
				t_depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				t_depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				t_depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				t_depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				t_depthAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				t_depthAttachment.isDepth = true;

				// Add attachments to config
				config.attachments.push_back(t_colorAttachment); // index 0
				config.attachments.push_back(t_depthAttachment); // index 1

				// Subpass setup
				SubpassConfig t_subpass{};
				t_subpass.colorAttachmentIndices = { 0 };
				t_subpass.depthAttachmentIndex = 1;

				config.subpasses.push_back(t_subpass);

				// Dependency
				VkSubpassDependency dependency;
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				dependency.srcAccessMask = 0;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				config.dependencies.push_back(dependency);

				// Extent
				config.extent = t_extent;
				config.setViewportAndScissor = true;
				config.useSwapChainFramebuffers = true;
				config.createOwnFramebuffers = false;
				config.setResizeCallback = true;

				m_sceneRenderPass = new VulkanRenderPass(t_device, a_renderer);

				m_sceneRenderPass->Create(config);
			}

			void VulkanRenderPassManager::RecordRenderPasses(RecordRenderPassinfo& a_info,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors)
			{
				std::set<VulkanRenderPass*> t_visited;
				std::vector<VulkanRenderPass*> t_sortedPasses;

				//the whole resolving this is kind of pointless atm, maybe usefull in the future
				ResolveDependencies(m_sceneRenderPass, t_visited, t_sortedPasses);
				for (VulkanRenderPass* t_pass : m_renderPasses)
				{
					ResolveDependencies(t_pass, t_visited, t_sortedPasses);
				}

				for (VulkanRenderPass* t_pass : t_sortedPasses)
				{
					t_pass->RecordRenderPass(a_info, a_vertexBuffers, a_indexBuffers, a_nbIndices, a_descriptors);
				}
			}

			void VulkanRenderPassManager::RunSceneRenderPass(RecordRenderPassinfo& a_info,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors)
			{
				m_sceneRenderPass->RecordRenderPass(a_info, a_vertexBuffers, a_indexBuffers, a_nbIndices, a_descriptors);
			}

			void VulkanRenderPassManager::ResolveDependencies(VulkanRenderPass* a_pass,
				std::set<VulkanRenderPass*>& a_visited, std::vector<VulkanRenderPass*>& a_sorted)
			{
				if (a_visited.contains(a_pass)) return;

				for (VulkanRenderPass* t_dep : a_pass->GetDependencies())
				{
					ResolveDependencies(t_dep, a_visited, a_sorted);
				}

				a_visited.insert(a_pass);
				a_sorted.push_back(a_pass);
			}

			void VulkanRenderPassManager::SetSceneRenderPass(VulkanRenderPass* a_renderPass)
			{
				if (m_sceneRenderPass)
				{
					delete m_sceneRenderPass;
				}
				m_sceneRenderPass->SetParent(this);
				m_sceneRenderPass = a_renderPass;
			}

			void VulkanRenderPassManager::AddRenderPass(VulkanRenderPass* a_renderPass)
			{
				a_renderPass->SetParent(this);
				m_renderPasses.push_back(a_renderPass);
			}

			bool VulkanRenderPassManager::HasFlag(RenderPassFlags a_flag)
			{
				return std::ranges::find(m_renderPassFlags, a_flag) != m_renderPassFlags.end();
			}

			void VulkanRenderPassManager::AddFlag(RenderPassFlags a_flag)
			{
				if (!HasFlag(a_flag))
				{
					m_renderPassFlags.push_back(a_flag);
				}
			}

			void VulkanRenderPassManager::RemoveFlag(RenderPassFlags a_flag)
			{
				if (auto t_it = std::ranges::find(m_renderPassFlags, a_flag); t_it != m_renderPassFlags.end())
				{
					m_renderPassFlags.erase(t_it);
				}
			}


			/**
			 * Inserts a memory barrier to force synchronisation between two render passes.
			 * Only really suited for letting my imgui ui pass wait on the scene pass, to expand on later
			 * @param a_pass the previous pass the next pass is going to depend on
			 * @param a_dependentPass the next pass that is dependent on the previous pass
			 * @param a_buffer the current command buffer
			 */
			void VulkanRenderPassManager::InsertPipelineBarrier(const VulkanRenderPass* a_pass,
				const VulkanRenderPass* a_dependentPass, const VkCommandBuffer a_buffer) const
			{
				const uint32_t t_currentFrame = m_renderer->GetSwapChain()->GetCurrentFrame();

				VkImageMemoryBarrier t_barrier{};
				t_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				t_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				t_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				
				if (a_pass->GetConfig().dependencyImageLayoutOverride != VK_IMAGE_LAYOUT_UNDEFINED)
				{
					t_barrier.oldLayout = a_dependentPass->GetConfig().dependencyImageLayoutOverride;
					t_barrier.newLayout = a_dependentPass->GetConfig().dependencyImageLayoutOverride;
				}
				else
				{
					t_barrier.oldLayout = a_pass->GetConfig().attachments[0].finalLayout;
					t_barrier.newLayout = a_dependentPass->GetConfig().attachments[0].initialLayout;
				}
				t_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				t_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				t_barrier.image = a_pass->GetAttachmentResources()[t_currentFrame].image;
				t_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				t_barrier.subresourceRange.baseMipLevel = 0;
				t_barrier.subresourceRange.levelCount = 1;
				t_barrier.subresourceRange.baseArrayLayer = 0;
				t_barrier.subresourceRange.layerCount = 1;

				vkCmdPipelineBarrier(
					a_buffer,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &t_barrier);
			}

			VulkanRenderPass::VulkanRenderPass(const VkDevice a_device, Renderer* a_renderer)
			{
				m_device = a_device;
				m_renderer = a_renderer;
			}

			VulkanRenderPass::~VulkanRenderPass()
			{
				Destroy();
			}

			void VulkanRenderPass::Create(const RenderPassConfig& a_config)
			{
				m_config = a_config;

				std::vector<VkAttachmentDescription> t_vkAttachments;
                t_vkAttachments.reserve(a_config.attachments.size());

                for (const auto& attachment : a_config.attachments)
                {
					VkAttachmentDescription t_desc{};
					t_desc.format = attachment.format;
					t_desc.samples = VK_SAMPLE_COUNT_1_BIT;
					t_desc.loadOp = attachment.loadOp;
					t_desc.storeOp = attachment.storeOp;
					t_desc.stencilLoadOp = attachment.isDepth ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					t_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					t_desc.initialLayout = attachment.initialLayout;
					t_desc.finalLayout = attachment.finalLayout;

                    t_vkAttachments.push_back(t_desc);
                }

				//prepare subpass descriptions
				std::vector<VkSubpassDescription> t_vkSubpasses;
				std::vector<std::vector<VkAttachmentReference>> t_colorAttachmentRefs;
				std::vector<VkAttachmentReference> t_depthAttachmentRefs;

                for (const auto& t_subpass : a_config.subpasses)
                {
					std::vector<VkAttachmentReference> colorRefs;
                    for (uint32_t t_idx : t_subpass.colorAttachmentIndices)
                    {
						VkAttachmentReference t_ref{};
						t_ref.attachment = t_idx;
						t_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						colorRefs.push_back(t_ref);
                    }

					VkAttachmentReference t_depthRef{};
                    if (t_subpass.depthAttachmentIndex >= 0)
                    {
						t_depthRef.attachment = t_subpass.depthAttachmentIndex;
						t_depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    }

					t_colorAttachmentRefs.push_back(colorRefs);

					VkSubpassDescription subpassDesc{};
					subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
					subpassDesc.colorAttachmentCount = static_cast<uint32_t>(colorRefs.size());
					subpassDesc.pColorAttachments = t_colorAttachmentRefs.back().data();
					subpassDesc.pDepthStencilAttachment = t_subpass.depthAttachmentIndex >= 0 ? &t_depthRef : nullptr;

					if (t_subpass.depthAttachmentIndex >= 0)
						t_depthAttachmentRefs.push_back(t_depthRef);

					t_vkSubpasses.push_back(subpassDesc);
                }

				VkRenderPassCreateInfo t_renderPassInfo{};
				t_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				t_renderPassInfo.attachmentCount = static_cast<uint32_t>(t_vkAttachments.size());
				t_renderPassInfo.pAttachments = t_vkAttachments.data();
				t_renderPassInfo.subpassCount = static_cast<uint32_t>(t_vkSubpasses.size());
				t_renderPassInfo.pSubpasses = t_vkSubpasses.data();
				t_renderPassInfo.dependencyCount = static_cast<uint32_t>(a_config.dependencies.size());
				t_renderPassInfo.pDependencies = a_config.dependencies.data();

				VK_CHECK(vkCreateRenderPass(m_device, &t_renderPassInfo, nullptr, &m_renderPass), "Failed to create render pass!");

				if (!m_config.useSwapChainFramebuffers && m_config.createOwnFramebuffers)
				{
					CreateAttachments();
					CreateFramebuffers();
				}
				if (m_config.setResizeCallback)
				{
					VulkanSwapchain* swapchain = m_renderer->GetSwapChain()->CastVulkan();
					swapchain->AddResizeCallback([this](VkExtent2D a_extent)
						{
							m_config.extent = a_extent;
							if (m_config.createOwnFramebuffers)
								RecreateFrameBuffer(a_extent);
						});
					
				}
					
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

			void VulkanRenderPass::CreateFramebuffers()
			{
				if (m_attachmentResources.empty())
				{
					LOG_ERROR("Tried to create framebuffers with no attachments created!");
					return;
				}
				m_framebuffers.resize(m_attachmentResources.size());
				for (size_t i = 0; i < m_attachmentResources.size(); ++i)
				{
					std::vector<VkImageView> t_attachments;
					t_attachments.push_back(m_attachmentResources[i].view);

					if (m_isDepth) //if we have a depth resource, assign it
						t_attachments.push_back(m_depthAttachmentResource.view);

					VkFramebufferCreateInfo t_framebufferInfo{};

					t_framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					t_framebufferInfo.renderPass = m_renderPass;
					t_framebufferInfo.attachmentCount = static_cast<uint32_t>(t_attachments.size());
					t_framebufferInfo.pAttachments = t_attachments.data();
					t_framebufferInfo.width = m_config.extent.width;
					t_framebufferInfo.height = m_config.extent.height;
					t_framebufferInfo.layers = 1;

					VK_CHECK(vkCreateFramebuffer(m_device, &t_framebufferInfo, nullptr, &m_framebuffers[i]), "Failed to create framebuffer!");
				}

			}



			//assumes for now we only really have two attachments, depth and colour, and creates the
			//Colour attachment with triple buffering in mind
			void VulkanRenderPass::CreateAttachments()
			{

				m_attachmentResources.resize(m_config.attachments.size());
				for (size_t i = 0; i < m_config.attachments.size(); ++i)
				{
					if (const RenderPassAttachment& t_info = m_config.attachments[i]; t_info.isDepth)
						CreateDepthAttachment(t_info);
					else
					{
						uint32_t maxFrames = m_renderer->GetSwapChain()->CastVulkan()->GetMaxFrame();
						m_attachmentResources.resize(maxFrames);
						for (uint32_t j = 0; j < maxFrames; ++j)
						{
							CreateAttachment(t_info, j);
						}
					}
					
				}

				const VkDevice t_logicalDevice = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
				VkPhysicalDevice t_physicalDevice = m_renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();

				//we only need to create a sampler once so no need to recreate it
				if (m_sampler == VK_NULL_HANDLE)
					VulkanImage::CreateSampler(&m_sampler, t_logicalDevice, t_physicalDevice);
			}

			void VulkanRenderPass::RecordRenderPass(RecordRenderPassinfo& a_info,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors)
			{
				VkCommandBuffer t_commandBuffer = a_info.renderer->GetCommandPool()->CastVulkan()->mCommandBuffers[a_info.commandPoolIndex][a_info.currentFrame];
				Begin(t_commandBuffer, a_info.imageIndex, a_info.currentFrame);
				RunPipelineDrawFuncs(a_info, a_vertexBuffers, a_indexBuffers, a_nbIndices, a_descriptors);
				if (m_drawFunc)
					m_drawFunc(a_info, a_vertexBuffers, a_indexBuffers, a_nbIndices, a_descriptors);
				End(t_commandBuffer);
				
			}

			void VulkanRenderPass::Begin(const VkCommandBuffer a_cmd, const uint32_t a_imageIndex, const uint32_t a_currentFrame, VkSubpassContents a_contents) const
			{
				const VkExtent2D t_swapChainExtent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
				std::vector<VkClearValue> t_clearValues;
				t_clearValues.resize(m_config.attachments.size());

				for (size_t i = 0; i < m_config.attachments.size(); ++i)
				{
					const auto& t_info = m_config.attachments[i];

					if (t_info.aspectMask & VK_IMAGE_ASPECT_COLOR_BIT)
					{
						t_clearValues[i].color = { {0.467f, 0.71f, 1.f, 0.996f} };
					}
					else if (t_info.aspectMask & VK_IMAGE_ASPECT_DEPTH_BIT)
					{
						t_clearValues[i].depthStencil = { .depth = 1.0f, .stencil = 0 };
					}
				}

				VkRenderPassBeginInfo t_beginInfo{};
				t_beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				t_beginInfo.renderPass = m_renderPass;
				if (m_config.useSwapChainFramebuffers && !m_config.createOwnFramebuffers)
					t_beginInfo.framebuffer = m_renderer->GetSwapChain()->CastVulkan()->GetFramebuffers()[a_imageIndex];
				else
					t_beginInfo.framebuffer = m_framebuffers[a_currentFrame];
				t_beginInfo.renderArea.offset = { 0, 0 };
				t_beginInfo.renderArea.extent = m_config.extent;
				t_beginInfo.clearValueCount = static_cast<uint32_t>(t_clearValues.size());
				t_beginInfo.pClearValues = t_clearValues.data();

				vkCmdBeginRenderPass(a_cmd, &t_beginInfo, VK_SUBPASS_CONTENTS_INLINE);

				//set viewport and scissor if configured to do so
				if (m_config.setViewportAndScissor)
				{
					float t_offsetWidth = (static_cast<float>(t_swapChainExtent.width) - static_cast<float>(m_config.extent.width))/2;
					float t_offsetHeight = (static_cast<float>(t_swapChainExtent.height) - static_cast<float>(m_config.extent.height))/2;

					VkViewport viewport;
					viewport.x = -t_offsetWidth;
					viewport.y = -t_offsetHeight;
					viewport.width = static_cast<float>(t_swapChainExtent.width);
					viewport.height = static_cast<float>(t_swapChainExtent.height);
					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;
					vkCmdSetViewport(a_cmd, 0, 1, &viewport);

					VkRect2D scissor;
					scissor.offset = {0,0};
					scissor.extent = m_config.extent;
					vkCmdSetScissor(a_cmd, 0, 1, &scissor);
				}
			}

			void VulkanRenderPass::End(const VkCommandBuffer a_cmd)
			{
				vkCmdEndRenderPass(a_cmd);
			}

			void VulkanRenderPass::SetDrawFunc(
				std::function<void(RecordRenderPassinfo&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>&)> a_func)
			{
				m_drawFunc = std::move(a_func);
			}

			void VulkanRenderPass::RecreateFrameBuffer(const VkExtent2D a_extent)
			{
				//set the new extent
				m_config.extent = a_extent;

				//if using swapchain images, only recreate framebuffers
				//cleanup old framebuffers
				if (m_config.useSwapChainFramebuffers && m_config.createOwnFramebuffers)
				{
					DestroyFramebuffers();

					//create framebuffers
					std::vector<VkImageView> t_imageViews = m_renderer->GetSwapChain()->CastVulkan()->GetImageViews();
					std::vector<std::vector<VkImageView>> t_imageViews2;
					for (auto& t_imageView : t_imageViews)
					{
						t_imageViews2.push_back({ t_imageView });
					}
					CreateFramebuffers(t_imageViews2);
				}
				//if not using swapchain images, recreate framebuffers and attachments
				else if (m_config.createOwnFramebuffers)
				{
					DestroyFramebuffers();
					DestroyAttachments();
					//create framebuffers
					CreateAttachments();
					CreateFramebuffers();
				}
				
			}

			void VulkanRenderPass::Destroy()
			{
				if (m_renderPass != VK_NULL_HANDLE)
				{
					vkDestroyRenderPass(m_device, m_renderPass, nullptr);
					m_renderPass = VK_NULL_HANDLE;
				}

				
				//cleanup framebuffer and attachments
				DestroyFramebuffers();
				DestroyAttachments();
				
				//destroy sampler
				if (m_sampler != VK_NULL_HANDLE)
				{
					vkDestroySampler(m_device, m_sampler, nullptr);
					m_sampler = VK_NULL_HANDLE;
				}
				m_attachmentResources.clear();
			}

			void VulkanRenderPass::DestroyFramebuffers()
			{
				for (auto& t_framebuffer : m_framebuffers)
				{
					if (t_framebuffer != VK_NULL_HANDLE)
					{
						vkDestroyFramebuffer(m_device, t_framebuffer, nullptr);
						t_framebuffer = VK_NULL_HANDLE;
					}
				}
				m_framebuffers.clear();
			}

			void VulkanRenderPass::DestroyAttachments()
			{
				for (auto& [image, memory, view] : m_attachmentResources)
				{
					if (view != VK_NULL_HANDLE) {
						vkDestroyImageView(m_device, view, nullptr);
						view = VK_NULL_HANDLE;
					}
					if (image != VK_NULL_HANDLE) {
						vkDestroyImage(m_device, image, nullptr);
						image = VK_NULL_HANDLE;
					}
					if (memory != VK_NULL_HANDLE) {
						vkFreeMemory(m_device, memory, nullptr);
						memory = VK_NULL_HANDLE;
					}
				}
				m_attachmentResources.clear();

				//cleanup depth attachment
				if (m_depthAttachmentResource.view != VK_NULL_HANDLE)
				{
					vkDestroyImageView(m_device, m_depthAttachmentResource.view, nullptr);
					m_depthAttachmentResource.view = VK_NULL_HANDLE;

				}
				if (m_depthAttachmentResource.image != VK_NULL_HANDLE)
				{
					vkDestroyImage(m_device, m_depthAttachmentResource.image, nullptr);
					m_depthAttachmentResource.image = VK_NULL_HANDLE;
				}
				if (m_depthAttachmentResource.memory != VK_NULL_HANDLE)
				{
					vkFreeMemory(m_device, m_depthAttachmentResource.memory, nullptr);
					m_depthAttachmentResource.memory = VK_NULL_HANDLE;
				}
			}

			void VulkanRenderPass::CreateDepthAttachment(const RenderPassAttachment& a_attachment)
			{
				m_isDepth = true;

				VkImageCreateInfo t_imageInfo = {};
				t_imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				t_imageInfo.imageType = VK_IMAGE_TYPE_2D;
				t_imageInfo.format = a_attachment.format;
				t_imageInfo.extent.width = m_config.extent.width;
				t_imageInfo.extent.height = m_config.extent.height;
				t_imageInfo.extent.depth = 1;
				t_imageInfo.mipLevels = 1;
				t_imageInfo.arrayLayers = 1;
				t_imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				t_imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				t_imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

				if (a_attachment.isDepth)
					t_imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

				t_imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				t_imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				// Create image
				VK_CHECK(vkCreateImage(m_device, &t_imageInfo, nullptr, &m_depthAttachmentResource.image), "Failed to create attachment image")

				// Allocate memory
				VkMemoryRequirements t_memRequirements;
				vkGetImageMemoryRequirements(m_device, m_depthAttachmentResource.image, &t_memRequirements);

				VkMemoryAllocateInfo t_allocInfo = {};
				t_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				t_allocInfo.allocationSize = t_memRequirements.size;
				t_allocInfo.memoryTypeIndex = VulkanBuffer::FindMemoryType(t_memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice());

				VK_CHECK(vkAllocateMemory(m_device, &t_allocInfo, nullptr, &m_depthAttachmentResource.memory), "Failed to allocate image memory")

				vkBindImageMemory(m_device, m_depthAttachmentResource.image, m_depthAttachmentResource.memory, 0);

				// Create image view
				VkImageViewCreateInfo t_viewInfo = {};
				t_viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				t_viewInfo.image = m_depthAttachmentResource.image;
				t_viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				t_viewInfo.format = a_attachment.format;
				t_viewInfo.subresourceRange.aspectMask = a_attachment.aspectMask;
				t_viewInfo.subresourceRange.baseMipLevel = 0;
				t_viewInfo.subresourceRange.levelCount = 1;
				t_viewInfo.subresourceRange.baseArrayLayer = 0;
				t_viewInfo.subresourceRange.layerCount = 1;

				VK_CHECK(vkCreateImageView(m_device, &t_viewInfo, nullptr, &m_depthAttachmentResource.view), "Failed to create image view!")
			}

			void VulkanRenderPass::CreateAttachment(const RenderPassAttachment& a_attachment, uint32_t a_index)
			{
				VkImageCreateInfo t_imageInfo = {};
				t_imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				t_imageInfo.imageType = VK_IMAGE_TYPE_2D;
				t_imageInfo.format = a_attachment.format;
				t_imageInfo.extent.width = m_config.extent.width;
				t_imageInfo.extent.height = m_config.extent.height;
				t_imageInfo.extent.depth = 1;
				t_imageInfo.mipLevels = 1;
				t_imageInfo.arrayLayers = 1;
				t_imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				t_imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				t_imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

				if (a_attachment.isDepth)
					t_imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

				t_imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				t_imageInfo.initialLayout = a_attachment.initialLayout;
				

				// Create image
				VK_CHECK(vkCreateImage(m_device, &t_imageInfo, nullptr, &m_attachmentResources[a_index].image), "Failed to create attachment image");

				// Allocate memory
				VkMemoryRequirements t_memRequirements;
				vkGetImageMemoryRequirements(m_device, m_attachmentResources[a_index].image, &t_memRequirements);

				VkMemoryAllocateInfo t_allocInfo = {};
				t_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				t_allocInfo.allocationSize = t_memRequirements.size;
				t_allocInfo.memoryTypeIndex = VulkanBuffer::FindMemoryType(t_memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice());

				VK_CHECK(vkAllocateMemory(m_device, &t_allocInfo, nullptr, &m_attachmentResources[a_index].memory), "Failed to allocate image memory");

				vkBindImageMemory(m_device, m_attachmentResources[a_index].image, m_attachmentResources[a_index].memory, 0);

				// Create image view
				VkImageViewCreateInfo t_viewInfo = {};
				t_viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				t_viewInfo.image = m_attachmentResources[a_index].image;
				t_viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				t_viewInfo.format = a_attachment.format;
				t_viewInfo.subresourceRange.aspectMask = a_attachment.aspectMask;
				t_viewInfo.subresourceRange.baseMipLevel = 0;
				t_viewInfo.subresourceRange.levelCount = 1;
				t_viewInfo.subresourceRange.baseArrayLayer = 0;
				t_viewInfo.subresourceRange.layerCount = 1;

				VK_CHECK(vkCreateImageView(m_device, &t_viewInfo, nullptr, &m_attachmentResources[a_index].view), "Failed to create image view")

				
			}

			void VulkanRenderPass::RunPipelineDrawFuncs(RecordRenderPassinfo& a_info,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors)
			{
				for (VulkanGraphicPipeline* t_graphicPipeline : m_graphicPipelines)
				{
					if (t_graphicPipeline)
						t_graphicPipeline->CallDrawFunc(a_info, a_vertexBuffers, a_indexBuffers, a_nbIndices, a_descriptors);
				}
			}
		}
	}
}
