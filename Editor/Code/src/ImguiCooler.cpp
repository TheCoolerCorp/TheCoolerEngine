#include  "ImguiCooler.h"


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"


void UI::Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer)
{
	
	uint32_t g_QueueFamily = ImGui_ImplVulkanH_SelectQueueFamilyIndex(renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice());

	wd.Surface = renderer->GetSurface()->CastVulkan()->GetVkSurfaceKHR();
	//wd.Swapchain = renderer->GetSwapChain()->CastVulkan()->GetSwapChain();
	wd.ImageCount = 0;


	// RESET MY SWAPCHAIN

	VkBool32 res;
	vkGetPhysicalDeviceSurfaceSupportKHR(renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice(), g_QueueFamily, wd.Surface, &res);
	const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_R8G8B8_SRGB };
	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

	wd.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice(), wd.Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
	LOG_ERROR(renderer->GetSwapChain()->CastVulkan()->GetExtent2D().width);
	LOG_ERROR(renderer->GetSwapChain()->CastVulkan()->GetExtent2D().height);

	ImGui_ImplVulkanH_CreateOrResizeWindow(renderer->GetInstance()->CastVulkan()->GetVkInstance(), 
		renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice(), 
		renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(),
		&wd, g_QueueFamily,
		nullptr, renderer->GetSwapChain()->CastVulkan()->GetExtent2D().width,
		renderer->GetSwapChain()->CastVulkan()->GetExtent2D().height, renderer->GetSwapChain()->CastVulkan()->GetMaxFrame());

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	ImGui::StyleColorsDark();


	VkDescriptorPool t_descriptorPool = VK_NULL_HANDLE;

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
	VK_CHECK(vkCreateDescriptorPool(renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), &pool_info, nullptr, &t_descriptorPool), "Failed to create imgui descriptor pool");


	ImGui_ImplGlfw_InitForVulkan(window->CastGLFW()->GetWindow(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = renderer->GetInstance()->CastVulkan()->GetVkInstance();
	init_info.PhysicalDevice = renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
	init_info.Device = renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	init_info.Queue = renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue();
	init_info.DescriptorPool = t_descriptorPool;
	init_info.MinImageCount = renderer->GetSwapChain()->GetMaxFrame();
	init_info.ImageCount = renderer->GetSwapChain()->GetMaxFrame();
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.RenderPass = renderer->GetRenderPass()->CastVulkan()->GetRenderPass();
	init_info.Subpass = renderer->GetRenderPass()->CastVulkan()->GetSubpassesCount();

	ImGui_ImplVulkan_Init(&init_info);
}

void UI::Update(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");
	ImGui::End();

	bool b = true;
	ImGui::ShowDemoWindow(&b);

	ImGui::Render();

}

void UI::Destroy(Engine::Core::Renderer* renderer)
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


}

