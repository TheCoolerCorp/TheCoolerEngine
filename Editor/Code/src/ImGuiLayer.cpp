#include "../Inlude/ImGuiLayer.h"
#include "../Inlude/VulkanImGui.h"
#include "Core/Renderer/Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

void ImGuiLayer::OnAttach(Engine::Core::Window::IWindow* window)
{
	Layer::OnAttach(window);
	m_imGui = new VulkanImGui(m_renderer);
	m_imGui->SetImGuiParent(this);
	m_imGui->Init(window, m_renderer);
}

void ImGuiLayer::OnDetach()
{
	if (m_imGui)
	{
		delete m_imGui;
		m_imGui = nullptr;
	}
	Layer::OnDetach();
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
	Layer::OnUpdate(deltaTime);
}

void ImGuiLayer::OnUiRender()
{
	Layer::OnUiRender();
	m_imGui->NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Begin("Viewport");
	m_imGui->DrawSceneAsImage();
	ImGui::End();
	m_imGui->Render();

}