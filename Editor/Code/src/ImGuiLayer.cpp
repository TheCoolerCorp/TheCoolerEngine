#include "ImGuiLayer.h"

//ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <VulkanImGuiRenderer.h>

void ImGuiLayer::Init(IWindow* window, Renderer* renderer)
{
	m_ImGuiRenderer = new VulkanImGuiRenderer();
	m_ImGuiRenderer->Init(window, renderer);
}

void ImGuiLayer::Update()
{
	NewFrame();
	//render stuff here idk
	Render();
}

void ImGuiLayer::Destroy()
{
}

void ImGuiLayer::NewFrame()
{
	m_ImGuiRenderer->NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::ImGuiDraw()
{
	ImGui::ShowDemoWindow();
}

void ImGuiLayer::Render()
{
	m_ImGuiRenderer->Render();
	ImGui::Render();
	
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
	return m_ImGuiRenderer->GetDrawData();
}