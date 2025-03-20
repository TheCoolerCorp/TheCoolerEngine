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
			}


			void VulkanRenderPass::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
                vkDestroyRenderPass(a_logicalDevice->CastVulkan()->GetVkDevice(), m_renderPass, nullptr);
			}
		}
	}
}