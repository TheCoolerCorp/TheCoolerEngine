#include "VulkanImGuiRenderer.h"

#include "vector"

//ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

//Vulkan includes
#include <Core/GraphicsAPI/Vulkan/VulkanUtils.h>
#include <Core/Logger/Logger.h>
#include <Core/GraphicsAPI/Vulkan/QueueFamilies.h>

using namespace Engine::Core::GraphicsAPI;

static void check_vk_result(VkResult err)
{
    if (err == VK_SUCCESS)
        return;
    
    if (err < 0)
        LOG_CRITICAL("[Vulkan] Error: Error during ImGUI initialisation \n");
}

void VulkanImGuiRenderer::Init(IWindow* window, Renderer* renderer)
{
	
    GLFWwindow* glWindow = window->CastGLFW()->GetWindow();

	VkInstance g_Instance = renderer->GetApiInstance()->CastVulkan()->GetVkInstance();
	VkPhysicalDevice g_PhysicalDevice = renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
	VkDevice g_Device = renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	
    QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(g_PhysicalDevice, renderer->GetSurface()->CastVulkan()->GetVkSurfaceKHR());
	uint32_t g_QueueFamily = indices.GetGraphicsFamily().value();
	
    VkQueue g_Queue = renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue();
	VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
	VkDescriptorPool g_DescriptorPool = renderer->GetDescriptorPool()->CastVulkan()->GetDescriptorPool();
	VkRenderPass g_RenderPass = renderer->GetRenderPass()->CastVulkan()->GetRenderPass();
	uint32_t g_MinImageCount = renderer->GetSwapChain()->CastVulkan()->GetMaxFrame();
	uint32_t g_ImageCount = g_MinImageCount;
	VkAllocationCallbacks* g_Allocator = nullptr;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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
    init_info.DescriptorPool = g_DescriptorPool;
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
}

void VulkanImGuiRenderer::Render()
{
}
