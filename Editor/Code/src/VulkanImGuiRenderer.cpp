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
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

//glfw includes
#include <GLFW/glfw3.h>

#include <utility>

#include "GamePlay/Camera.h"

using namespace Engine::Core::GraphicsAPI;
using namespace Engine::Core::Window;
using namespace Engine::Core;

static void check_vk_result(VkResult err)
{
    if (err == VK_SUCCESS)
        return;
    
    if (err < 0)
        LOG_CRITICAL("[Vulkan] Error: Error in ImGUI, code " + std::to_string(err) + '\n');
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
	uint32_t g_MinImageCount = swapChain->GetMaxFrame();
	uint32_t g_ImageCount = g_MinImageCount;
	VkAllocationCallbacks* g_Allocator = nullptr;

	VulkanRenderPass::AddRenderPassFlag(FLAG_OVERRIDE_DEFAULT_RENDERPASS);
	VulkanRenderPass::SetSceneRenderPass([this](VkRecordCommandBufferInfo info, std::vector<Engine::GamePlay::GameObjectData> data, std::vector<VkCommandBuffer>& buffers)
		{
			this->SceneRenderPassImGui(info, std::move(data), buffers);
		});
	VulkanRenderPass::AddRenderPass([this](VkRecordCommandBufferInfo info, std::vector<Engine::GamePlay::GameObjectData> data, std::vector<VkCommandBuffer>& buffers)
		{
			this->ViewportRenderPass(info, std::move(data), buffers);
		}, 1);

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

    CreateViewportImage();
    CreateViewportImageViews();
    CreateViewportCommandBuffer();
    CreateViewportRenderPass();
    CreateViewportFrameBuffers();
	VulkanCommandPool::CreateCommandPool(&m_ViewportCommandPool, m_renderer);
    CreateViewportPipeline();
	CreateSceneImageDescriptorSets();
}

void VulkanImGuiRenderer::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void VulkanImGuiRenderer::RenderDrawData(VkRecordCommandBufferInfo info)
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

void VulkanImGuiRenderer::CreateSceneImageDescriptorSets()
{
	ClearDescriptorSets();
	VulkanSwapchain* swapchain = m_renderer->GetSwapChain()->CastVulkan();
	std::vector<VkImageView> imageViews = swapchain->GetImageViews();
	std::vector<VkSampler> samplers = swapchain->GetSamplers();
	m_Dset.resize(imageViews.size());
	for (uint32_t i = 0; i < imageViews.size(); i++)
		m_Dset[i] = ImGui_ImplVulkan_AddTexture(samplers[i], imageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

}

void VulkanImGuiRenderer::DrawSceneAsImage()
{
	//VulkanSwapchain* swapchain = m_renderer->GetSwapChain()->CastVulkan();
 //   ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
 //   ImGui::Image(reinterpret_cast<ImTextureID>(m_Dset[swapchain->GetCurrentFrame()]), ImVec2{ viewportPanelSize.x, viewportPanelSize.y });

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

void VulkanImGuiRenderer::CreateViewportImageViews()
{
	m_ViewportImageViews.resize(m_ViewportImages.size());
	VkDevice a_logicalDevice = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	for (uint32_t i = 0; i < m_ViewportImages.size(); i++)
	{
		VulkanImage::CreateImageView(m_ViewportImages[i], &m_ViewportImageViews[i], a_logicalDevice, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void VulkanImGuiRenderer::CreateViewportCommandBuffer()
{
	m_ViewportCommandBuffers.resize(m_ViewportImageViews.size());

	VkCommandPool commandPool = m_renderer->GetCommandPool()->CastVulkan()->GetVkCommandPool();

	VkDevice t_device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = (uint32_t)m_ViewportCommandBuffers.size();
	if (vkAllocateCommandBuffers(t_device, &allocInfo, m_ViewportCommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void VulkanImGuiRenderer::CreateViewportFrameBuffers()
{
	m_ViewportFramebuffers.resize(m_ViewportImageViews.size());
	VkExtent2D t_extent = m_renderer->GetSwapChain()->CastVulkan()->GetExtent2D();
	VkDevice t_device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	VkImageView t_depthImageView = m_renderer->GetSwapChain()->CastVulkan()->GetDepthImageView();
	for (size_t i = 0; i < m_ViewportImageViews.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {
				m_ViewportImageViews[i],
				t_depthImageView };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_ViewportRenderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = t_extent.width;
		framebufferInfo.height = t_extent.height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(t_device, &framebufferInfo, nullptr, &m_ViewportFramebuffers[i]), "Failed to create framebuffer!")
	}
}

void VulkanImGuiRenderer::CreateViewportRenderPass()
{
	std::array<VkAttachmentDescription, 2> attachments = {};
	VkFormat m_SwapChainImageFormat = m_renderer->GetSwapChain()->CastVulkan()->GetImageFormat();
	VkDevice m_Device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	VkFormat m_DepthFormat = m_renderer->GetSwapChain()->CastVulkan()->GetDepthFormat();
	// Color attachment
	attachments[0].format = m_SwapChainImageFormat;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	// Depth attachment
	attachments[1].format = m_DepthFormat;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;
	subpassDescription.pDepthStencilAttachment = &depthReference;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = nullptr;
	subpassDescription.pResolveAttachments = nullptr;

	// Subpass dependencies for layout transitions
	std::array<VkSubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpassDescription;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	if (vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_ViewportRenderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}
}

void VulkanImGuiRenderer::CreateViewportPipeline()
{
	VkDevice a_logicalDevice = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	VkDescriptorSetLayout a_cameraDescriptor = m_renderer->GetPipeline()->CastVulkan()->GetCameraDescriptorSetLayout();
	VkDescriptorSetLayout a_objectDescriptor = m_renderer->GetPipeline()->CastVulkan()->GetObjectDescriptorSetLayout();
	VkPipelineLayout a_layout = m_renderer->GetPipeline()->CastVulkan()->GetLayout();
	// Shader creation
	VkShaderModule t_vertexShader = VulkanGraphicPipeline::CreateShader("Assets/Shaders/vert.spv", a_logicalDevice);
	VkShaderModule t_fragmentShader = VulkanGraphicPipeline::CreateShader("Assets/Shaders/frag.spv", a_logicalDevice);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = t_vertexShader;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = t_fragmentShader;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Vertex setup
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	auto bindingDescription = Engine::Resource::VulkanVertexSpec::getBindingDescription();
	auto attributeDescriptions = Engine::Resource::VulkanVertexSpec::getAttributeDescriptions();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();


	// Pipeline spec
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// Multisampling disable
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Depth activate by default, need to be modular later
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE; // Set to true if uncomment below
	//colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	//colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	//colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	//colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	//colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	//colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	std::vector<VkDynamicState> dynamicStates =
	{
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkDescriptorSetLayoutBinding camUBOLayoutBinding{};
	camUBOLayoutBinding.binding = 0;
	camUBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	camUBOLayoutBinding.descriptorCount = 1;
	camUBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	camUBOLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo camUBOLayoutInfo{};
	camUBOLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	camUBOLayoutInfo.bindingCount = 1;
	camUBOLayoutInfo.pBindings = &camUBOLayoutBinding;

	VK_CHECK(vkCreateDescriptorSetLayout(a_logicalDevice, &camUBOLayoutInfo, nullptr, &a_cameraDescriptor),
		"Failed to create Camera UBO descriptor set layout");

	VkDescriptorSetLayoutBinding objUBOLayoutBinding{};
	objUBOLayoutBinding.binding = 0;
	objUBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	objUBOLayoutBinding.descriptorCount = 1;
	objUBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	objUBOLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	std::array<VkDescriptorSetLayoutBinding, 2> objBindings = { objUBOLayoutBinding, samplerLayoutBinding };

	VkDescriptorSetLayoutCreateInfo objUBOLayoutInfo{};
	objUBOLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	objUBOLayoutInfo.bindingCount = static_cast<uint32_t>(objBindings.size());
	objUBOLayoutInfo.pBindings = objBindings.data();

	VK_CHECK(vkCreateDescriptorSetLayout(a_logicalDevice, &objUBOLayoutInfo, nullptr, &a_objectDescriptor),
		"Failed to create Object UBO + Sampler descriptor set layout");

	std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = { a_cameraDescriptor, a_objectDescriptor };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

	VK_CHECK(vkCreatePipelineLayout(a_logicalDevice, &pipelineLayoutInfo, nullptr, &a_layout),
		"Failed to create pipeline layout");

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = a_layout;
	pipelineInfo.renderPass = m_ViewportRenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.pDepthStencilState = &depthStencil;

	VK_CHECK(vkCreateGraphicsPipelines(a_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_ViewportPipeline),
		"Failed to create graphic pipeline");


	// Destroy shader, already load don't need to store them
	vkDestroyShaderModule(a_logicalDevice, t_vertexShader, nullptr);
	vkDestroyShaderModule(a_logicalDevice, t_fragmentShader, nullptr);

}

void VulkanImGuiRenderer::CreateViewportImage()
{
	VulkanSwapchain* swapchain = m_renderer->GetSwapChain()->CastVulkan();
	size_t swapchainImageCount = swapchain->GetImages().size();
	VkExtent2D m_SwapChainExtent = swapchain->GetExtent2D();
	VkDevice m_Device = m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice();
	VkPhysicalDevice physicalDevice = m_renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice();
	VkCommandPool commandPool = m_renderer->GetCommandPool()->CastVulkan()->GetVkCommandPool();
	m_ViewportImages.resize(swapchainImageCount);
	m_DstImageMemory.resize(swapchainImageCount);

	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		// Create the linear tiled destination image to copy to and to read the memory from
		VkImageCreateInfo imageCreateCI{};
		imageCreateCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
		// Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
		imageCreateCI.format = VK_FORMAT_R8G8B8A8_UNORM;
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

		VkCommandBuffer copyCmd = VulkanCommandPool::BeginSingleTimeCommands(m_Device, commandPool);

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

		VulkanCommandPool::EndSingleTimeCommands(copyCmd, commandPool, m_Device, m_renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue());
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
	std::vector<Engine::GamePlay::GameObjectData> objectsData, std::vector<VkCommandBuffer>& buffers)
{
	uint32_t currentFrame = m_renderer->GetSwapChain()->CastVulkan()->GetCurrentFrame();
	VkExtent2D m_SwapChainExtent = info.swapChain->GetExtent2D();
	VkPipelineLayout t_layout = info.graphicPipeline->GetLayout();

	const VkCommandBuffer commandBuffer = m_ViewportCommandBuffers[info.currentFrame];
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// beginInfo.flags = 0;									// Optional
	// beginInfo.pInheritanceInfo = nullptr; // Optional


	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo), "ImGui Scene Render Pass: Failed to begin recording command buffer")

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

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_ViewportPipeline);

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_SwapChainExtent.width;
	viewport.height = (float)m_SwapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = m_SwapChainExtent;

	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkDescriptorSet t_cameraDescriptorSet = info.camera->GetDescriptor()->CastVulkan()->GetDescriptorSet();
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 0, 1, &t_cameraDescriptorSet, 0, nullptr);


	// render scene
	for (auto t_gameObjectData : objectsData)
	{
		VkBuffer t_vertexBuffer = t_gameObjectData.mVertexBuffer->CastVulkan()->GetBuffer();
		constexpr VkDeviceSize t_offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &t_vertexBuffer, t_offsets);

		vkCmdBindIndexBuffer(commandBuffer, t_gameObjectData.mIndexBuffer->CastVulkan()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 1, 1, &t_gameObjectData.mDescriptor->CastVulkan()->GetDescriptorSets()[info.imageIndex], 0, nullptr);

		vkCmdDrawIndexed(commandBuffer, t_gameObjectData.mNbIndices, 1, 0, 0, 0);
	}
	vkCmdEndRenderPass(commandBuffer);

	VK_CHECK(vkEndCommandBuffer(commandBuffer), "failed to record command buffer!")

	buffers.push_back(commandBuffer);
}

void VulkanImGuiRenderer::ViewportRenderPass(VkRecordCommandBufferInfo info,
	std::vector<Engine::GamePlay::GameObjectData> objectsData, std::vector<VkCommandBuffer>& buffers)
{
	const VkExtent2D t_swapChainExtent = info.swapChain->GetExtent2D();
	const VkPipeline t_pipeline = info.graphicPipeline->GetPipeline();
	const VkPipelineLayout t_layout = info.graphicPipeline->GetLayout();

	VkCommandBufferBeginInfo t_beginInfo{};
	t_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VK_CHECK(vkBeginCommandBuffer(info.commandBuffer, &t_beginInfo), "failed to begin recording command buffer!");


	VkRenderPassBeginInfo t_renderPassInfo{};
	t_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	t_renderPassInfo.renderPass = info.renderPass;
	t_renderPassInfo.framebuffer = info.swapChain->GetFramebuffers()[info.imageIndex];
	t_renderPassInfo.renderArea.offset = { .x = 0, .y = 0 };
	t_renderPassInfo.renderArea.extent = t_swapChainExtent;

	std::array<VkClearValue, 2> t_clearValues{};
	t_clearValues[0].color = { {0.467f, 0.71f, 1.f, 0.996f} };
	t_clearValues[1].depthStencil = { .depth = 1.0f, .stencil = 0 };

	t_renderPassInfo.clearValueCount = static_cast<uint32_t>(t_clearValues.size());
	t_renderPassInfo.pClearValues = t_clearValues.data();

	vkCmdBeginRenderPass(info.commandBuffer, &t_renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_pipeline);

	VkViewport t_viewport;
	t_viewport.x = 0.0f;
	t_viewport.y = 0.0f;
	t_viewport.width = static_cast<float>(info.swapChain->GetExtent2D().width);
	t_viewport.height = static_cast<float>(t_swapChainExtent.height);
	t_viewport.minDepth = 0.0f;
	t_viewport.maxDepth = 1.0f;
	vkCmdSetViewport(info.commandBuffer, 0, 1, &t_viewport);

	VkRect2D t_scissor;
	t_scissor.offset = { .x = 0, .y = 0 };
	t_scissor.extent = t_swapChainExtent;
	vkCmdSetScissor(info.commandBuffer, 0, 1, &t_scissor);

	VkDescriptorSet t_cameraDescriptorSet = info.camera->GetDescriptor()->CastVulkan()->GetDescriptorSet();
	vkCmdBindDescriptorSets(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_layout, 0, 1, &t_cameraDescriptorSet, 0, nullptr);

	RenderDrawData(info);

	//VulkanRenderPass::RunRenderCallbacks(info.commandBuffer);

	vkCmdEndRenderPass(info.commandBuffer);

	VK_CHECK(vkEndCommandBuffer(info.commandBuffer), "failed to end command buffer!");

	buffers.push_back(info.commandBuffer);
}
