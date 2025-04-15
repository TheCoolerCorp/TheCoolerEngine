#include "../Inlude/RHIImGui.h"
#include "../Inlude/VulkanImGui.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/Renderer/Renderer.h"

using namespace Engine::Core;
using namespace Engine::Core::GraphicsAPI;

static void check_vk_result(VkResult err)
{
	if (err == VK_SUCCESS)
		return;

	if (err < 0)
		LOG_CRITICAL("[Vulkan] Error: Error in ImGUI, code " + std::to_string(err) + '\n');
}

VulkanImGui::VulkanImGui(Engine::Core::Renderer* renderer): RHIImGui(renderer)
{
	GraphicsAPI::VulkanRenderPassManager::AddFlag(GraphicsAPI::FLAG_VK_RHI_OVERRIDE_DEFAULT_RENDERPASS);
}

void VulkanImGui::Init(Window::IWindow* window, Renderer* renderer)
{
	m_renderer = renderer;
	GLFWwindow* glWindow = window->CastGLFW()->GetWindow();

	VkInstance g_Instance = renderer->GetApiInstance()->CastVulkan()->GetVkInstance();
	VkPhysicalDevice g_PhysicalDevice = renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
	VkDevice g_Device = renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	VkSurfaceKHR g_Surface = renderer->GetSurface()->CastVulkan()->GetVkSurfaceKHR();
	GraphicsAPI::QueueFamilyIndices indices = GraphicsAPI::QueueFamilyIndices::FindQueueFamilies(g_PhysicalDevice, g_Surface);
	uint32_t g_QueueFamily = indices.GetGraphicsFamily().value();
	VkQueue g_Queue = renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue();
	VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
	VkRenderPass g_RenderPass = renderer->GetRenderPass()->CastVulkan()->GetSceneRenderPass()->GetRenderPass();
	GraphicsAPI::VulkanSwapchain* swapChain = renderer->GetSwapChain()->CastVulkan();
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

void VulkanImGui::SetupRenderPasses()
{
	SetupSceneRenderPass();
	SetupImGuiRenderPass();
}

void VulkanImGui::SetupSceneRenderPass()
{


	VkFormat imageFormat = m_renderer->GetSwapChain()->CastVulkan()->GetImageFormat();
	VkExtent2D extent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
	VkDevice device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

	RenderPassConfig config{};

	// Color attachment setup
	RenderPassAttachment colorAttachment{};
	colorAttachment.format = imageFormat;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	colorAttachment.isDepth = false;

	// Depth attachment setup
	RenderPassAttachment depthAttachment{};
	depthAttachment.format = m_renderer->GetPhysicalDevice()->CastVulkan()->FindSupportedFormat(
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

	m_imGuiRenderPass = new VulkanRenderPass(device, m_renderer);

	m_imGuiRenderPass->Create(config);
	m_imGuiRenderPass->CreateAttachments();
	//m_sceneRenderPass->SetFramebuffers(m_renderer->GetSwapChain()->CastVulkan()->GetFramebuffers());

	m_imGuiRenderPass->SetDrawFunc(
		[this](RecordRenderPassinfo a_info, const std::vector<RHI::IRenderObject*>& a_renderObjects,
			const std::vector<RHI::IBuffer*>& a_vertexBuffers,
			const std::vector<RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices)
		{
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//TEMPORARY: MAKE RENDERPASS OPTIONALLY STORE REFERENCE TO ITS ASSOCIATED PIPELINE LATER
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			vkCmdBindPipeline(a_info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_renderer->GetPipeline()->CastVulkan()->GetPipeline());

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

void VulkanImGui::SetupImGuiRenderPass()
{
}

void VulkanImGui::NewFrame()
{
}

void VulkanImGui::Render()
{
}

void VulkanImGui::DrawSceneAsImage()
{
}
