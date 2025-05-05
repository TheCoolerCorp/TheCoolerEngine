#include "RHIImGui.h"
#include "VulkanImGui.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanRenderPass.h"
#include "Core/Renderer/Renderer.h"
#include "ImGuiLayer.h"

using namespace Engine::Core;
using namespace Engine::Core::GraphicsAPI;

static void check_vk_result(VkResult err)
{
	if (err == VK_SUCCESS)
		return;

	if (err < 0)
		LOG_CRITICAL("[Vulkan] Error: Error in ImGUI, code " + std::to_string(err) + '\n');
}
namespace Editor::EditorLayer::Ui
{
	VulkanImGui::VulkanImGui(Renderer* a_renderer): RHIImGui(a_renderer), m_viewportWindowExtent(), m_pool(nullptr)
	{
	}

	VulkanImGui::~VulkanImGui()
	{
		for (VkDescriptorSet& dset : m_dset)
		{
			ImGui_ImplVulkan_RemoveTexture(dset);
		}
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		if (m_pool)
		{
			vkDestroyDescriptorPool(m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), m_pool, nullptr);
		}
		
	}

	void VulkanImGui::Init(Window::IWindow* a_window, Renderer* a_renderer)
	{
		m_renderer = a_renderer;
		GLFWwindow* glWindow = a_window->CastGLFW()->GetWindow();

		VkInstance g_Instance = a_renderer->GetApiInstance()->CastVulkan()->GetVkInstance();
		VkPhysicalDevice g_PhysicalDevice = a_renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
		VkDevice g_Device = a_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
		VkSurfaceKHR g_Surface = a_renderer->GetSurface()->CastVulkan()->GetVkSurfaceKHR();
		GraphicsAPI::QueueFamilyIndices indices = GraphicsAPI::QueueFamilyIndices::FindQueueFamilies(g_PhysicalDevice, g_Surface);
		uint32_t g_QueueFamily = indices.GetGraphicsFamily().value();
		VkQueue g_Queue = a_renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue();
		VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
		GraphicsAPI::VulkanSwapchain* swapChain = a_renderer->GetSwapChain()->CastVulkan();
		uint32_t g_MinImageCount = swapChain->GetMaxFrame();
		uint32_t g_ImageCount = g_MinImageCount;
		VkAllocationCallbacks* g_Allocator = nullptr;

		SetupRenderPasses();
		VulkanSwapchain* t_swapChain = a_renderer->GetSwapChain()->CastVulkan();
		t_swapChain->AddResizeCallback(
			[this](VkExtent2D a_extent)
			{
				this->RecreateSceneImageDescriptorSets(a_extent);
			}
		);

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
		init_info.RenderPass = m_imGuiViewportRenderPass->GetRenderPass();
		init_info.Subpass = 0;
		init_info.MinImageCount = g_MinImageCount;
		init_info.ImageCount = g_ImageCount;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = g_Allocator;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info);

		
		CreateSceneImageDescriptorSets();
	}

	void VulkanImGui::Update()
	{
		//if (m_viewportWindowResized)
		//{
		//	if (m_viewportWindowExtent.height <= 0 || m_viewportWindowExtent.width <= 0)
		//		return;
		//	const VkDevice t_device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

		//	vkDeviceWaitIdle(t_device);

		//	m_viewportWindowResized = false;
		//	m_imGuiRenderPass->RecreateFrameBuffer(m_viewportWindowExtent);
		//	RecreateSceneImageDescriptorSets(m_viewportWindowExtent);
		//}
	}

	void VulkanImGui::SetupRenderPasses()
	{
		SetupSceneRenderPass();
		SetupImGuiRenderPass();
		m_renderer->GetSwapChain()->CastVulkan()->AddResizeCallback(
			[this](VkExtent2D a_extent)
			{
				this->RecreateSceneImageDescriptorSets(m_viewportWindowExtent);
			}
		);
	}

	void VulkanImGui::SetupSceneRenderPass()
	{


		VkFormat imageFormat = m_renderer->GetSwapChain()->CastVulkan()->GetImageFormat();
		VkExtent2D extent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
		VkDevice device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

		RenderPassConfig t_config{};

		// Color attachment setup
		RenderPassAttachment t_colorAttachment;
		t_colorAttachment.format = imageFormat;
		t_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		t_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		t_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		t_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		t_colorAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		t_colorAttachment.isDepth = false;

		// Depth attachment setup
		RenderPassAttachment t_depthAttachment;
		t_depthAttachment.format = m_renderer->GetPhysicalDevice()->CastVulkan()->FindSupportedFormat(
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
		t_config.attachments.push_back(t_colorAttachment); // index 0
		t_config.attachments.push_back(t_depthAttachment); // index 1

		// Subpass setup
		SubpassConfig subpass{};
		subpass.colorAttachmentIndices = { 0 };
		subpass.depthAttachmentIndex = 1;

		t_config.subpasses.push_back(subpass);

		// Dependency
		VkSubpassDependency dependency;
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		t_config.dependencies.push_back(dependency);

		// Extent
		t_config.extent = extent;
		t_config.setViewportAndScissor = true;
		t_config.useSwapChainFramebuffers = false;
		//we handle the resize for this one
		t_config.setResizeCallback = true;

		m_imGuiRenderPass = new VulkanRenderPass(device, m_renderer);

		m_imGuiRenderPass->Create(t_config);
		//m_sceneRenderPass->SetFramebuffers(m_renderer->GetSwapChain()->CastVulkan()->GetFramebuffers());


		VulkanRenderPassManager* t_manager = m_renderer->GetRenderPass()->CastVulkan();
		t_manager->SetSceneRenderPass(m_imGuiRenderPass);
	}

	void VulkanImGui::SetupImGuiRenderPass()
	{
		m_imGuiViewportRenderPass = new VulkanRenderPass(m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), m_renderer);
		VkFormat t_imageFormat = m_renderer->GetSwapChain()->CastVulkan()->GetImageFormat();
		VkExtent2D t_extent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
		VkDevice t_device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

		RenderPassConfig t_config{};

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
		t_depthAttachment.format = m_renderer->GetPhysicalDevice()->CastVulkan()->FindSupportedFormat(
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
		t_config.attachments.push_back(t_colorAttachment); // index 0
		t_config.attachments.push_back(t_depthAttachment); // index 1

		// Subpass setup
		SubpassConfig t_subpass{};
		t_subpass.colorAttachmentIndices = { 0 };
		t_subpass.depthAttachmentIndex = 1;

		t_config.subpasses.push_back(t_subpass);

		// Dependency
		VkSubpassDependency dependency;
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		t_config.dependencies.push_back(dependency);

		// Extent
		t_config.extent = t_extent;
		t_config.setViewportAndScissor = true;
		t_config.useSwapChainFramebuffers = true;
		t_config.createOwnFramebuffers = false;
		t_config.setResizeCallback = true;

		m_imGuiViewportRenderPass->Create(t_config);
		m_imGuiViewportRenderPass->SetDrawFunc([this](RecordRenderPassinfo& a_info,
			std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
			std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
			std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
			std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors)
			{
				VkCommandBuffer t_commandBuffer = a_info.renderer->GetCommandPool()->CastVulkan()->m_commandBuffers[a_info.commandPoolIndex][a_info.currentFrame];
				//m_imguiLayer->OnUiRender();
				ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), t_commandBuffer);
			}
		);
		m_imGuiViewportRenderPass->AddDependency(m_imGuiRenderPass);
		//std::vector<VkImageView> t_imageViews = m_renderer->GetSwapChain()->CastVulkan()->GetImageViews();
		//std::vector<std::vector<VkImageView>> t_imageViews2;
		//for (auto& t_imageView : t_imageViews)
		//{
		//	t_imageViews2.push_back({t_imageView});
		//}
		//m_imGuiViewportRenderPass->CreateFramebuffers(t_imageViews2);

		

		VulkanRenderPassManager* t_manager = m_renderer->GetRenderPass()->CastVulkan();
		t_manager->AddRenderPass(m_imGuiViewportRenderPass);
	}

	void VulkanImGui::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void VulkanImGui::Render()
	{
		ImGui::Render();
	}

	void VulkanImGui::DrawSceneAsImage()
	{
		VulkanSwapchain* t_swapchain = m_renderer->GetSwapChain()->CastVulkan();
		const ImVec2 t_viewportPanelSize = ImGui::GetContentRegionAvail();
		VkExtent2D t_extent = { static_cast<uint32_t>(t_viewportPanelSize.x), static_cast<uint32_t>(t_viewportPanelSize.y) };
		if (m_viewportWindowExtent.width != t_extent.width || m_viewportWindowExtent.height != t_extent.height)
		{
			m_viewportWindowExtent = t_extent;
			if (m_viewportWindowExtent.height <= 0 || m_viewportWindowExtent.width <= 0 || m_viewportWindowExtent.height >= 3000 || m_viewportWindowExtent.width >= 3000)
				return;
			const VkDevice t_device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

			vkDeviceWaitIdle(t_device);

			m_viewportWindowResized = false;
			//m_imGuiRenderPass->RecreateFrameBuffer(m_viewportWindowExtent);
			
			//m_viewportWindowResized = true;
			//RecreateSceneImageDescriptorSets(m_viewportWindowExtent);
		}
		float t_offsetX = (static_cast<float>(t_swapchain->GetExtent2D().width) - t_viewportPanelSize.x) / 2;
		float t_offsetY = (static_cast<float>(t_swapchain->GetExtent2D().height) - t_viewportPanelSize.y) / 2;
		ImGui::SetCursorPos(ImVec2{ -t_offsetX, -t_offsetY });
		ImGui::Image(reinterpret_cast<ImTextureID>(m_dset[t_swapchain->GetCurrentFrame()]), ImVec2{ static_cast<float>(t_swapchain->GetExtent2D().width), static_cast<float>(t_swapchain->GetExtent2D().height) });
	}

	void VulkanImGui::Cleanup()
	{
	}

	void VulkanImGui::CreateDescriptorPool(const VkDevice a_device)
	{
		VkDescriptorPoolSize t_poolSizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 0;
		for (VkDescriptorPoolSize& pool_size : t_poolSizes)
			pool_info.maxSets += pool_size.descriptorCount;
		pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(t_poolSizes));
		pool_info.pPoolSizes = t_poolSizes;
		VK_CHECK(vkCreateDescriptorPool(a_device, &pool_info, nullptr, &m_pool), "Failed to create descriptor pool!")
	}

	void VulkanImGui::CreateSceneImageDescriptorSets()
	{
		const std::vector<AttachmentResource>& t_attachmentResources = m_imGuiRenderPass->GetAttachmentResources();
		m_dset.resize(t_attachmentResources.size());
		for (uint32_t i = 0; i < t_attachmentResources.size(); i++)
			m_dset[i] = ImGui_ImplVulkan_AddTexture(m_imGuiRenderPass->GetSampler(), t_attachmentResources[i].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void VulkanImGui::RecreateSceneImageDescriptorSets(VkExtent2D a_extent)
	{
		//cleanup old descriptor sets
		for (VkDescriptorSet& dset : m_dset)
		{
			ImGui_ImplVulkan_RemoveTexture(dset);
		}
		CreateSceneImageDescriptorSets();
	}

	void VulkanImGui::SetWindowExtent(VkExtent2D a_extent)
	{
		m_viewportWindowExtent = a_extent;
		m_viewportWindowResized = true;
	}
}
