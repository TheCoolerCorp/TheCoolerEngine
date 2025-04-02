#include "VulkanImGuiRenderer.h"


//ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

//Vulkan includes
#include <vulkan/vulkan.h>
#include "Core/Renderer/Renderer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Logger/Logger.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"

//glfw includes
#include <GLFW/glfw3.h>

using namespace Engine::Core::GraphicsAPI;
using namespace Engine::Core::Window;
using namespace Engine::Core;

static void check_vk_result(VkResult err)
{
    if (err == VK_SUCCESS)
        return;
    
    if (err < 0)
        LOG_CRITICAL("[Vulkan] Error: Error during ImGUI initialisation " + std::to_string(err) + '\n');
}

void VulkanImGuiRenderer::Init(IWindow* window, Renderer* renderer)
{
    m_renderer = renderer;
    GLFWwindow* glWindow = window->CastGLFW()->GetWindow();

	VkInstance g_Instance = renderer->GetApiInstance()->CastVulkan()->GetVkInstance();
	VkPhysicalDevice g_PhysicalDevice = renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
	VkDevice g_Device = renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	VkSurfaceKHR g_Surface = renderer->GetSurface()->CastVulkan()->GetVkSurfaceKHR();
    QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(g_PhysicalDevice, g_Surface);
	uint32_t g_QueueFamily = indices.GetGraphicsFamily().value();
    VkQueue g_Queue = renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue();
	VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
	VkRenderPass g_RenderPass = renderer->GetRenderPass()->CastVulkan()->GetRenderPass();
	VulkanSwapchain* swapChain = renderer->GetSwapChain()->CastVulkan();
    VulkanRenderPass::AddRenderPass([renderer](VkRecordCommandBufferInfo info, std::vector<Engine::GamePlay::GameObjectData> data)
    {
    	RenderDrawData(info);
    }, 2);
	uint32_t g_MinImageCount = swapChain->GetMaxFrame();
	uint32_t g_ImageCount = g_MinImageCount;
	VkAllocationCallbacks* g_Allocator = nullptr;

    CreateDescriptorPool(g_Device);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(glWindow, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    //init_info.ApiVersion = VK_API_VERSION_1_3;              // Pass in your value of VkApplicationInfo::apiVersion, otherwise will default to header version.
    init_info.Instance = g_Instance;
    init_info.PhysicalDevice = g_PhysicalDevice;
    init_info.Device = g_Device;
    init_info.QueueFamily = g_QueueFamily;
    init_info.Queue = g_Queue;
    init_info.PipelineCache = g_PipelineCache;
    init_info.DescriptorPool = m_pool;
    init_info.RenderPass = g_RenderPass;
    init_info.Subpass = 0;
    init_info.MinImageCount = g_MinImageCount;
    init_info.ImageCount = g_ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = g_Allocator;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);


}

void VulkanImGuiRenderer::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void VulkanImGuiRenderer::RenderDrawData(Engine::Core::GraphicsAPI::VkRecordCommandBufferInfo info)
{
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), info.commandBuffer);
}

void VulkanImGuiRenderer::Render()
{
	ImGui::Render();
}

void VulkanImGuiRenderer::CreateDescriptorPool(VkDevice device)
{

        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 0;
        for (VkDescriptorPoolSize& pool_size : pool_sizes)
            pool_info.maxSets += pool_size.descriptorCount;
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        VkResult err = vkCreateDescriptorPool(device, &pool_info, nullptr, &m_pool);
        check_vk_result(err);
}

void VulkanImGuiRenderer::DrawSceneAsImage()
{
    ClearDescriptorSets();
	VulkanSwapchain* swapchain = m_renderer->GetSwapChain()->CastVulkan();
	std::vector<VkImageView> imageViews = swapchain->GetImageViews();
	std::vector<VkSampler> samplers = swapchain->GetSamplers();
    m_Dset.resize(imageViews.size());
    for (uint32_t i = 0; i < imageViews.size(); i++)
        m_Dset[i] = ImGui_ImplVulkan_AddTexture(samplers[i], imageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(reinterpret_cast<ImTextureID>(m_Dset[swapchain->GetCurrentFrame()]), ImVec2{ viewportPanelSize.x, viewportPanelSize.y });

}

void VulkanImGuiRenderer::ClearDescriptorSets()
{
    for (int a = 0; a<m_Dset.size(); a++)
    {
        ImGui_ImplVulkan_RemoveTexture(m_Dset[a]);
    }
}

ImDrawData* VulkanImGuiRenderer::GetDrawData()
{
	return m_drawData;
}

void VulkanImGuiRenderer::createViewportImage()
{
	VulkanSwapchain* swapchain = m_renderer->GetSwapChain()->CastVulkan();
	size_t swapchainImageCount = swapchain->GetImages().size();
	VkExtent2D m_SwapChainExtent = swapchain->GetExtent2D();
	VkDevice m_Device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	VkPhysicalDevice physicalDevice = m_renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
	m_ViewportImages.resize(swapchainImageCount);
	m_DstImageMemory.resize(swapchainImageCount);

	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		// Create the linear tiled destination image to copy to and to read the memory from
		VkImageCreateInfo imageCreateCI{};
		imageCreateCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
		// Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
		imageCreateCI.format = VK_FORMAT_B8G8R8A8_SRGB;
		imageCreateCI.extent.width = m_SwapChainExtent.width;
		imageCreateCI.extent.height = m_SwapChainExtent.height;
		imageCreateCI.extent.depth = 1;
		imageCreateCI.arrayLayers = 1;
		imageCreateCI.mipLevels = 1;
		imageCreateCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateCI.tiling = VK_IMAGE_TILING_LINEAR;
		imageCreateCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		// Create the image
		// VkImage dstImage;
		vkCreateImage(m_Device, &imageCreateCI, nullptr, &m_ViewportImages[i]);
		// Create memory to back up the image
		VkMemoryRequirements memRequirements;
		VkMemoryAllocateInfo memAllocInfo{};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		// VkDeviceMemory dstImageMemory;
		vkGetImageMemoryRequirements(m_Device, m_ViewportImages[i], &memRequirements);
		memAllocInfo.allocationSize = memRequirements.size;
		// Memory must be host visible to copy from
		memAllocInfo.memoryTypeIndex = VulkanBuffer::FindMemoryType(memRequirements.memoryTypeBits,
		                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		                                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, physicalDevice);
		vkAllocateMemory(m_Device, &memAllocInfo, nullptr, &m_DstImageMemory[i]);
		vkBindImageMemory(m_Device, m_ViewportImages[i], m_DstImageMemory[i], 0);

		VkCommandBuffer copyCmd = VulkanCommandPool::BeginSingleTimeCommands(m_Device,m_ViewportCommandPool);

		InsertImageMemoryBarrier(
			copyCmd,
			m_ViewportImages[i],
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		VulkanCommandPool::EndSingleTimeCommands(copyCmd, m_ViewportCommandPool, m_Device, m_renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue());
	}
}

void VulkanImGuiRenderer::createViewportImageViews()
{
	m_ViewportImageViews.resize(m_ViewportImages.size());

	for (uint32_t i = 0; i < m_ViewportImages.size(); i++)
	{
		m_ViewportImageViews[i] = VulkanSwapchain::CreateImageView(m_ViewportImages[i], VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice());
	}
}

void VulkanImGuiRenderer::InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer,
	VkImage image,
	VkAccessFlags srcAccessMask,
	VkAccessFlags dstAccessMask,
	VkImageLayout oldImageLayout,
	VkImageLayout newImageLayout,
	VkPipelineStageFlags srcStageMask,
	VkPipelineStageFlags dstStageMask,
	VkImageSubresourceRange subresourceRange)
{
	VkImageMemoryBarrier imageMemoryBarrier{};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.srcAccessMask = srcAccessMask;
	imageMemoryBarrier.dstAccessMask = dstAccessMask;
	imageMemoryBarrier.oldLayout = oldImageLayout;
	imageMemoryBarrier.newLayout = newImageLayout;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange = subresourceRange;

	vkCmdPipelineBarrier(
		cmdbuffer,
		srcStageMask,
		dstStageMask,
		0,
		0, nullptr,
		0, nullptr,
		1, &imageMemoryBarrier);
}

void VulkanImGuiRenderer::SceneRenderPassImGui(VkRecordCommandBufferInfo info,
	std::vector<Engine::GamePlay::GameObjectData> objectsData)
{
	uint32_t currentFrame = m_renderer->GetSwapChain()->CastVulkan()->GetCurrentFrame();
	VkExtent2D m_SwapChainExtent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
	VkPipelineLayout t_layout = info.graphicPipeline->GetLayout();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// beginInfo.flags = 0;									// Optional
	// beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(m_ViewportCommandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_ViewportRenderPass;
	renderPassInfo.framebuffer = m_ViewportFramebuffers[info.imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_SwapChainExtent;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(m_ViewportCommandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(m_ViewportCommandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_ViewportPipeline);

	// render scene
	for (auto t_gameObjectData : objectsData)
	{
		VkBuffer t_vertexBuffer = t_gameObjectData.mVertexBuffer->CastVulkan()->GetBuffer();
		constexpr VkDeviceSize t_offsets[] = { 0 };
		vkCmdBindVertexBuffers(info.commandBuffer, 0, 1, &t_vertexBuffer, t_offsets);

		vkCmdBindIndexBuffer(info.commandBuffer, t_gameObjectData.mIndexBuffer->CastVulkan()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 1, 1, &t_gameObjectData.mDescriptor->CastVulkan()->GetDescriptorSets()[info.imageIndex], 0, nullptr);

		vkCmdDrawIndexed(info.commandBuffer, t_gameObjectData.mNbIndices, 1, 0, 0, 0);
	}
	vkCmdEndRenderPass(m_ViewportCommandBuffers[currentFrame]);

	if (vkEndCommandBuffer(m_ViewportCommandBuffers[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}
