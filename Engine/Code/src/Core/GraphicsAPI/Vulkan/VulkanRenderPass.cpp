#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"


namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanRenderPass::Create(RHI::ISwapChain* a_swapChain, RHI::IPhysicalDevice* a_physicalDevice, RHI::ILogicalDevice* a_logicalDevice)
			{
                // VERSION 2
                std::vector<VkAttachmentDescription> descriptors;


                // Color spec
				VkAttachmentDescription colorAttachment{};
				colorAttachment.format = a_swapChain->CastVulkan()->GetImageFormat();
				colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                VkAttachmentReference colorAttachmentRef{};
                colorAttachmentRef.attachment = 0;
                colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


                // Depth spec
                VkAttachmentDescription depthAttachment{};
                depthAttachment.format = a_physicalDevice->CastVulkan()->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                                                                                                                VK_IMAGE_TILING_OPTIMAL,VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
                depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference depthAttachmentRef{};
                depthAttachmentRef.attachment = 1;
                depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


                descriptors.push_back(colorAttachment);
                descriptors.push_back(depthAttachment);

                // Subpasses 
                VkSubpassDescription depthSubpass{};
                depthSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                depthSubpass.colorAttachmentCount = 0;
                depthSubpass.pDepthStencilAttachment = &depthAttachmentRef;

                VkSubpassDescription colorSubpass{};
                colorSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                colorSubpass.colorAttachmentCount = 1;
                colorSubpass.pColorAttachments = &colorAttachmentRef;

                VkSubpassDependency depthSubpassDependency{};
                depthSubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
                depthSubpassDependency.dstSubpass = 0; 
                depthSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                depthSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                depthSubpassDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                depthSubpassDependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

                VkSubpassDependency colorSubpassDependency{};
                colorSubpassDependency.srcSubpass = 0;
                colorSubpassDependency.dstSubpass = 1;
                colorSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                colorSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                colorSubpassDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                colorSubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

                std::vector<VkSubpassDescription> subpasses = { depthSubpass, colorSubpass };
                std::vector<VkSubpassDependency> dependencies = { depthSubpassDependency, colorSubpassDependency };

                // Render pass creation
                VkRenderPassCreateInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<uint32_t>(descriptors.size());
				renderPassInfo.pAttachments = descriptors.data();
                renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
				renderPassInfo.pSubpasses = subpasses.data();
				renderPassInfo.dependencyCount = 1;
				renderPassInfo.pDependencies = dependencies.data();

                VK_CHECK(vkCreateRenderPass(a_logicalDevice->CastVulkan()->GetVkDevice(), &renderPassInfo, nullptr, &m_renderPass), "Failed to create swapchain");
                m_subpasses = static_cast<uint32_t>(subpasses.size());
			}


			void VulkanRenderPass::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
                vkDestroyRenderPass(a_logicalDevice->CastVulkan()->GetVkDevice(), m_renderPass, nullptr);
			}
		}
	}
}