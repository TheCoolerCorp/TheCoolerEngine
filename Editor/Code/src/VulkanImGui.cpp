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
	VulkanImGui::VulkanImGui(Engine::Core::Renderer* a_renderer): RHIImGui(a_renderer)
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
		if (m_viewportWindowResized)
		{
			if (m_viewportWindowExtent.height <= 0 || m_viewportWindowExtent.width <= 0)
				return;
			const VkDevice t_device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

			vkDeviceWaitIdle(t_device);

			m_viewportWindowResized = false;
			m_imGuiRenderPass->RecreateFrameBuffer(m_viewportWindowExtent);
			RecreateSceneImageDescriptorSets(m_viewportWindowExtent);
		}
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

		RenderPassConfig t_config{};

		// Color attachment setup
		RenderPassAttachment t_colorAttachment{};
		t_colorAttachment.format = imageFormat;
		t_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		t_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		t_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		t_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		t_colorAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		t_colorAttachment.isDepth = false;

		// Depth attachment setup
		RenderPassAttachment t_depthAttachment{};
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
		VkSubpassDependency dependency{};
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
		t_config.setResizeCallback = false;

		m_imGuiRenderPass = new VulkanRenderPass(device, m_renderer);

		m_imGuiRenderPass->Create(t_config);
		//m_sceneRenderPass->SetFramebuffers(m_renderer->GetSwapChain()->CastVulkan()->GetFramebuffers());

		m_imGuiRenderPass->SetDrawFunc(
			[this](const RecordRenderPassinfo& a_info, const std::vector<RHI::IObjectDescriptor*>& a_renderObjects,
				const std::vector<RHI::IBuffer*>& a_vertexBuffers,
				const std::vector<RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices)
			{
				//vkCmdBindPipeline(a_info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_renderer->GetPipeline()->CastVulkan()->GetPipeline());

				const VkPipelineLayout t_layout = a_info.graphicPipeline->GetLayout();

				//const VkDescriptorSet t_cameraDescriptorSet = a_info.camera->GetDescriptor()->CastVulkan()->GetDescriptorSet();
				//vkCmdBindDescriptorSets(a_info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 0, 1, &t_cameraDescriptorSet, 0, nullptr);

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

		VulkanRenderPassManager* t_manager = m_renderer->GetRenderPass()->CastVulkan();
		t_manager->SetSceneRenderPass(m_imGuiRenderPass);
	}

	void VulkanImGui::SetupImGuiRenderPass()
	{
		m_imGuiViewportRenderPass = new VulkanRenderPass(m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), m_renderer);
		VkExtent2D t_extent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
		VkFormat t_imageFormat = m_renderer->GetSwapChain()->CastVulkan()->GetImageFormat();


		RenderPassConfig t_config = {};
		t_config.attachments.push_back({
			.format = t_imageFormat,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.isDepth = false
			});

		t_config.extent = t_extent;

		VkAttachmentReference t_colorRef = {};
		t_colorRef.attachment = 0;
		t_colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		SubpassConfig subpassConfig{};
		subpassConfig.colorAttachmentIndices = { 0 };
		subpassConfig.depthAttachmentIndex = -1; //no depth

		t_config.subpasses.push_back(subpassConfig);

		VkSubpassDependency t_dependency = {};
		t_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		t_dependency.dstSubpass = 0;
		t_dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		t_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		t_dependency.srcAccessMask = 0;
		t_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		t_dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		t_config.dependencies.push_back(t_dependency);
		t_config.useSwapChainFramebuffers = true;
		t_config.createOwnFramebuffers = true;
		t_config.dependencyImageLayoutOverride = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		m_imGuiViewportRenderPass->Create(t_config);
		m_imGuiViewportRenderPass->SetDrawFunc([this](const RecordRenderPassinfo a_info, const std::vector<RHI::IObjectDescriptor*>& a_renderObjects,
			const std::vector<RHI::IBuffer*>& a_vertexBuffers,
			const std::vector<RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices)
			{
				m_imguiLayer->OnUiRender();
				ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), a_info.commandBuffer);
			}
		);
		m_imGuiViewportRenderPass->AddDependency(m_imGuiRenderPass);
		std::vector<VkImageView> t_imageViews = m_renderer->GetSwapChain()->CastVulkan()->GetImageViews();
		std::vector<std::vector<VkImageView>> t_imageViews2;
		for (auto& t_imageView : t_imageViews)
		{
			t_imageViews2.push_back({t_imageView});
		}
		m_imGuiViewportRenderPass->CreateFramebuffers(t_imageViews2);

		

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
			m_viewportWindowResized = true;
		}
		ImGui::Image(reinterpret_cast<ImTextureID>(m_dset[t_swapchain->GetCurrentFrame()]), ImVec2{ t_viewportPanelSize.x, t_viewportPanelSize.y });
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
