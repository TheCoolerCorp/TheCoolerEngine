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
    	vkQueueWaitIdle(renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue());
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
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
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
    std::vector<VkDescriptorSet> m_Dset;
	VulkanSwapchain* swapchain = m_renderer->GetSwapChain()->CastVulkan();
	std::vector<VkImageView> imageViews = swapchain->GetImageViews();
	std::vector<VkSampler> samplers = swapchain->GetSamplers();
    m_Dset.resize(imageViews.size());
    for (uint32_t i = 0; i < imageViews.size(); i++)
        m_Dset[i] = ImGui_ImplVulkan_AddTexture(samplers[i], imageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(reinterpret_cast<ImTextureID>(m_Dset[swapchain->GetCurrentFrame()]), ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
}

ImDrawData* VulkanImGuiRenderer::GetDrawData()
{
	return m_drawData;
}

