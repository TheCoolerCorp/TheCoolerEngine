#include "ImGuiLayer.h"

std::unique_ptr<RHIImGuiRenderer> ImGuiLayer::m_ImGuiRenderer;
std::unique_ptr<MainWindow> ImGuiLayer::m_MainWindow;
std::vector<std::shared_ptr<SubWindow>> ImGuiLayer::m_SubWindows;

void ImGuiLayer::Init(IWindow* window, Renderer* renderer)
{
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
