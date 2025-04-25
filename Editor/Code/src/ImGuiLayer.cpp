#include "ImGuiLayer.h"
#include "VulkanImGui.h"
#include "Core/Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "UiWindow.h"
namespace Editor::EditorLayer::Ui
{
	void ImGuiLayer::OnAttach(Engine::Core::Window::IWindow* a_window)
	{
		Layer::OnAttach(a_window);
		m_imGui = new VulkanImGui(m_renderer);
		m_imGui->SetImGuiParent(this);
		m_imGui->Init(a_window, m_renderer);
	}

	void ImGuiLayer::OnDetach()
	{

		Layer::OnDetach();
	}

	void ImGuiLayer::OnUpdate(float a_deltaTime)
	{
		Layer::OnUpdate(a_deltaTime);
		m_imGui->Update();
	}

	void ImGuiLayer::OnUiRender()
	{
		bool dockSpaceOpen = true;

		Layer::OnUiRender();
		m_imGui->NewFrame();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("DockSpace", &dockSpaceOpen, window_flags);
		ImGuiID t_dockSpaceID = ImGui::GetID("MainDockSpace");
		ImGuiDockNodeFlags t_dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGui::DockSpace(t_dockSpaceID, ImVec2(0.0f, 0.0f), t_dockspaceFlags);
		ImGui::End();
		ImGui::PopStyleVar(3);

		ImGui::Begin("Viewport");
		m_imGui->DrawSceneAsImage();
		ImGui::End();

		for (UiWindow* t_window : m_windows)
		{
			t_window->UiDraw();	
		}

		m_imGui->Render();

	}

	void ImGuiLayer::Delete()
	{
		if (m_imGui)
		{
			//m_imGui->Cleanup();
			delete m_imGui;
			m_imGui = nullptr;
		}
		//delete every UiWindow
		for (UiWindow* t_window : m_windows)
		{
			t_window->Destroy();
			delete t_window;
		}
		Layer::OnDetach();
	}

	
}
