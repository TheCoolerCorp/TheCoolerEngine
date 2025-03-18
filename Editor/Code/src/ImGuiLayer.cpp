#include "ImGuiLayer.h"


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
