#include "ImGuiLayer.h"

//ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <VulkanImGuiRenderer.h>

RHIImGuiRenderer* ImGuiLayer::m_imGuiRenderer;

ImGuiLayer::~ImGuiLayer()
{
	Destroy();
}

void ImGuiLayer::Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer)
{
	//we only have vulkan so its hardcoded for now
	m_imGuiRenderer = new VulkanImGuiRenderer;
	m_imGuiRenderer->Init(window, renderer);
	Engine::Core::GraphicsAPI::VulkanRenderPass::AddRenderPassCallback([this, renderer]
		{
			vkQueueWaitIdle(renderer->GetLogicalDevice()->CastVulkan()->GetGraphicsQueue());
			this->DrawUpdate();
		}, 1);
}

void ImGuiLayer::Update()
{
	
}

void ImGuiLayer::DrawUpdate()
{
	NewFrame();
	ImGuiDraw();
	Render();
}

void ImGuiLayer::Destroy()
{
	//delete m_imGuiRenderer;
	//for(auto& subWindow : m_subWindows)
	//{
	//	delete subWindow;
	//}
	//delete m_mainWindow;
}

void ImGuiLayer::NewFrame()
{
	m_imGuiRenderer->NewFrame();
}

void ImGuiLayer::ImGuiDraw()
{ 
	ImGui::ShowDemoWindow();
	Start("Scene");
	m_imGuiRenderer->DrawSceneAsImage();
	End();
}

void ImGuiLayer::Render()
{
	m_imGuiRenderer->Render();
}

void ImGuiLayer::Start(const char* name)
{
	ImGui::Begin(name);
}

void ImGuiLayer::End()
{
	ImGui::End();
}

ImDrawData* ImGuiLayer::GetDrawData()
{
	return m_imGuiRenderer->GetDrawData();
}