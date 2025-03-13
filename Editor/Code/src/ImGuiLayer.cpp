#include "ImGuiLayer.h"


void ImGuiLayer::Init()
{
	//Vulkan setup idk
	//or tell the RHI to do the setup we'll have to see
}

void ImGuiLayer::Update()
{
	NewFrame();
	//render stuff here idk
	Render();
}

void ImGuiLayer::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::Render()
{
	ImGui::Render();
	//TODO
	//Implement anything vulan-related
	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::Start(const char* name)
{
	ImGui::Begin(name);
}

void ImGuiLayer::End()
{
	ImGui::End();
}
