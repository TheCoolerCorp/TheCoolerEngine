#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCameraDescriptor.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescritptor.h"
#include "GamePlay/Camera.h"
#include "GamePlay/GameObject.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
            std::map<int, std::vector<std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>, std::vector<VkCommandBuffer>&)>>> VulkanRenderPass::m_renderPasses;
            //callbacks that get executed after a specified render pass id
            std::map<int, std::vector<std::function<void()>>> VulkanRenderPass::m_renderPassCallbacks;
			std::function<void(Core::GraphicsAPI::VkRecordCommandBufferInfo, std::vector<GamePlay::GameObjectData>, std::vector<VkCommandBuffer>&)> VulkanRenderPass::m_sceneRenderPass;
			int VulkanRenderPass::m_renderPassCount;
            int VulkanRenderPass::m_currentRenderCallback;

			std::vector<VulkanRenderPassFlags> VulkanRenderPass::m_renderPassFlags;

			std::string VulkanRenderPassFlagToString(VulkanRenderPassFlags flag)
            {
				switch (flag)
				{
				case FLAG_OVERRIDE_DEFAULT_RENDERPASS:
					return "FLAG_OVERRIDE_DEFAULT_RENDERPASS";
				default:
					return "UNKNOWN_FLAG";
				}
            }

			void VulkanRenderPass::Create(RHI::ISwapChain* a_swapChain, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice)
			{
                // VERSION 2
                std::vector<VkAttachmentDescription> t_descriptors;


                // Color spec
				VkAttachmentDescription t_colorAttachment{};
				t_colorAttachment.format = a_swapChain->CastVulkan()->GetImageFormat();
				t_colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
				t_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				t_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				t_colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				t_colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				t_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				t_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                VkAttachmentReference t_colorAttachmentRef{};
                t_colorAttachmentRef.attachment = 0;
                t_colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


                // Depth spec
                VkAttachmentDescription t_depthAttachment{};
                t_depthAttachment.format = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                                                                                                                VK_IMAGE_TILING_OPTIMAL,VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
                t_depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                t_depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                t_depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                t_depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                t_depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                t_depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                t_depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference t_depthAttachmentRef{};
                t_depthAttachmentRef.attachment = 1;
                t_depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


                t_descriptors.push_back(t_colorAttachment);
                t_descriptors.push_back(t_depthAttachment);

                // Subpasses 
                VkSubpassDescription t_subpass{};
                t_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                t_subpass.colorAttachmentCount = 1;
                t_subpass.pColorAttachments = &t_colorAttachmentRef;
                t_subpass.pDepthStencilAttachment = &t_depthAttachmentRef;

                VkSubpassDependency t_dependency{};
                t_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
                t_dependency.dstSubpass = 0;
                t_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                t_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                t_dependency.srcAccessMask = 0;
                t_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                t_dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

                std::vector<VkSubpassDescription> t_subpasses = { t_subpass };                
                std::vector<VkSubpassDependency> t_dependencies = { t_dependency, };

                // Render pass creation
                VkRenderPassCreateInfo t_renderPassInfo{};
				t_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				t_renderPassInfo.attachmentCount = static_cast<uint32_t>(t_descriptors.size());
				t_renderPassInfo.pAttachments = t_descriptors.data();
                t_renderPassInfo.subpassCount = static_cast<uint32_t>(t_subpasses.size());
				t_renderPassInfo.pSubpasses = t_subpasses.data();
				t_renderPassInfo.dependencyCount = 1;
				t_renderPassInfo.pDependencies = t_dependencies.data();

                VK_CHECK(vkCreateRenderPass(a_logicalDevice->CastVulkan()->GetVkDevice(), &t_renderPassInfo, nullptr, &m_renderPass), "Failed to create render pass !");
                m_subpasses = static_cast<uint32_t>(t_subpasses.size());

				if (!HasRenderPassFlag(FLAG_OVERRIDE_DEFAULT_RENDERPASS))
				{
					// Create the default render pass
					SetSceneRenderPass([this](VkRecordCommandBufferInfo info, std::vector<GamePlay::GameObjectData> objectsData, std::vector<VkCommandBuffer>& buffers)
						{
							this->RecordSceneRenderPass(info, objectsData, buffers);
						});
				}
			}


			void VulkanRenderPass::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
                vkDestroyRenderPass(a_logicalDevice->CastVulkan()->GetVkDevice(), m_renderPass, nullptr);
			}

			void VulkanRenderPass::RecordSceneRenderPass(VkRecordCommandBufferInfo info,
				std::vector<GamePlay::GameObjectData> objectsData, std::vector<VkCommandBuffer>& buffers)
			{
				const VkExtent2D t_swapChainExtent = info.swapChain->GetExtent2D();
				const VkPipeline t_pipeline = info.graphicPipeline->GetPipeline();
				const VkPipelineLayout t_layout = info.graphicPipeline->GetLayout();

				VkCommandBufferBeginInfo t_beginInfo{};
				t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

				VK_CHECK(vkBeginCommandBuffer(info.commandBuffer, &t_beginInfo), "failed to begin recording command buffer!");


				VkRenderPassBeginInfo t_renderPassInfo{};
				t_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				t_renderPassInfo.renderPass = info.renderPass;
				t_renderPassInfo.framebuffer = info.swapChain->GetFramebuffers()[info.imageIndex];
				t_renderPassInfo.renderArea.offset = { .x = 0, .y = 0 };
				t_renderPassInfo.renderArea.extent = t_swapChainExtent;

				std::array<VkClearValue, 2> t_clearValues{};
				t_clearValues[0].color = { {0.467f, 0.71f, 1.f, 0.996f} };
				t_clearValues[1].depthStencil = { .depth = 1.0f, .stencil = 0 };

				t_renderPassInfo.clearValueCount = static_cast<uint32_t>(t_clearValues.size());
				t_renderPassInfo.pClearValues = t_clearValues.data();

				vkCmdBeginRenderPass(info.commandBuffer, &t_renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_pipeline);

				VkViewport t_viewport;
				t_viewport.x = 0.0f;
				t_viewport.y = 0.0f;
				t_viewport.width = static_cast<float>(info.swapChain->GetExtent2D().width);
				t_viewport.height = static_cast<float>(t_swapChainExtent.height);
				t_viewport.minDepth = 0.0f;
				t_viewport.maxDepth = 1.0f;
				vkCmdSetViewport(info.commandBuffer, 0, 1, &t_viewport);

				VkRect2D t_scissor;
				t_scissor.offset = { .x = 0, .y = 0 };
				t_scissor.extent = t_swapChainExtent;
				vkCmdSetScissor(info.commandBuffer, 0, 1, &t_scissor);

				VkDescriptorSet t_cameraDescriptorSet = info.camera->GetDescriptor()->CastVulkan()->GetDescriptorSet();
				vkCmdBindDescriptorSets(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 0, 1, &t_cameraDescriptorSet, 0, nullptr);


				//VulkanRenderPass::RunRenderPass(info, objectsData);
				for (int i = 0; i < objectsData.size(); ++i)
				{
					GamePlay::GameObjectData t_gameObjectData = objectsData[i];
					VkBuffer t_vertexBuffer = t_gameObjectData.mVertexBuffer->CastVulkan()->GetBuffer();
					constexpr VkDeviceSize t_offsets[] = { 0 };
					vkCmdBindVertexBuffers(info.commandBuffer, 0, 1, &t_vertexBuffer, t_offsets);

					vkCmdBindIndexBuffer(info.commandBuffer, t_gameObjectData.mIndexBuffer->CastVulkan()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

					vkCmdBindDescriptorSets(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 1, 1, &t_gameObjectData.mDescriptor->CastVulkan()->GetDescriptorSets()[info.imageIndex], 0, nullptr);

					vkCmdDrawIndexed(info.commandBuffer, t_gameObjectData.mNbIndices, 1, 0, 0, 0);
				}

				vkCmdEndRenderPass(info.commandBuffer);

				VK_CHECK(vkEndCommandBuffer(info.commandBuffer), "failed to end command buffer!");
			}
		}
	}
}