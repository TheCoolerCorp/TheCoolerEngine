#include  "ImguiCooler.h"


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"


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

	Engine::Core::GraphicsAPI::QueueFamilyIndices t_indices = Engine::Core::GraphicsAPI::QueueFamilyIndices::FindQueueFamilies(renderer->GetPhysicalDevice()->CastVulkan()->GetVkPhysicalDevice(), renderer->GetSurface()->CastVulkan()->GetVkSurfaceKHR());

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = t_indices.GetGraphicsFamily().value();

	VK_CHECK(vkCreateCommandPool(renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), &poolInfo, nullptr, &m_commandPool), "Failed to create command pool");

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
	init_info.Subpass = 0;

	ImGui_ImplVulkan_Init(&init_info);

	m_commandBuffer = Engine::Core::GraphicsAPI::VulkanCommandPool::BeginSingleTimeCommands(renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), m_commandPool);
	ImGui_ImplVulkan_CreateFontsTexture();
	Engine::Core::GraphicsAPI::VulkanCommandPool::EndSingleTimeCommands(m_commandBuffer, m_commandPool, renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue());
}

void UI::Update(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	VK_CHECK(vkAllocateCommandBuffers(renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice(), &allocInfo, &commandBuffer), "Failed to allocate command buffer");

	// Begin Command Buffer
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin command buffer");

	VkFramebuffer t_currentFramebuffer = renderer->GetSwapChain()->CastVulkan()->GetFramebuffers()[renderer->GetSwapChain()->CastVulkan()->GetCurrentFrame()];
	VkExtent2D t_swapExtent = renderer->GetSwapChain()->CastVulkan()->GetExtent2D();

	// Begin render pass
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_renderPass->CastVulkan()->GetRenderPass();
	renderPassInfo.framebuffer = t_currentFramebuffer;  // Use the current framebuffer
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = { t_swapExtent.width, t_swapExtent.height };

	VkClearValue clearColor = { {0.0f, 0.0f, 0.0f, 1.0f} };  // Clear color: black
	renderPassInfo.pClearValues = &clearColor;
	renderPassInfo.clearValueCount = 1;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Record ImGui rendering commands
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Render();

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	// End render pass
	vkCmdEndRenderPass(commandBuffer);

	// End Command Buffer
	VK_CHECK(vkEndCommandBuffer(commandBuffer), "Failed to end command buffer");

	// Submit Command Buffer
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VK_CHECK(vkQueueSubmit(renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit command buffer");

	// Reset command buffer for next frame
	vkResetCommandBuffer(commandBuffer, 0);
}

