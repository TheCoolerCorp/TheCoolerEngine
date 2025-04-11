#include  "ImguiCooler.h"


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"


void UI::Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer)
{
	// Basic init 
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForVulkan(window->CastGLFW()->GetWindow(), true);

	// Empty RenderPass
	m_renderPass = new Engine::Core::GraphicsAPI::VulkanRenderPass;
	m_renderPass->CreateEmpty(renderer->GetSwapChain(), renderer->GetPhysicalDevice(), renderer->GetLogicalDevice());

	// Create DescriptorPool
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	VK_CHECK(vkCreateDescriptorPool(renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), &pool_info, nullptr, &m_pool), "failed");


	// Vulkan Init
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = renderer->GetInstance()->CastVulkan()->GetVkInstance();
	init_info.PhysicalDevice = renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
	init_info.Device = renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	init_info.Queue = renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue();
	init_info.DescriptorPool = m_pool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.RenderPass = m_renderPass->CastVulkan()->GetRenderPass();
	init_info.Subpass = m_renderPass->CastVulkan()->GetSubpassesCount();

	ImGui_ImplVulkan_Init(&init_info);
}

void UI::Update(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), );

	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	/*ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Render();*/
}

