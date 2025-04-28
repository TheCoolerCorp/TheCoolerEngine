#include "ImGuiLayer.h"
#include "VulkanImGui.h"
#include "Core/Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "../Include/Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "../Include/UiWindow.h"
#include "../Include/SceneGraphWindow.h"
#include "../Include/InspectorWindow.h"

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
		std::vector<int> t_indexesToDelete;
		for (UiWindow* t_window : m_windows)
		{
			if (t_window == nullptr)
				continue;
			if (t_window->IsClosed())
			{
				t_indexesToDelete.push_back(t_window->GetUid());
			}
		}
		for (int t_index : t_indexesToDelete)
		{
			UiWindow* t_window = m_windows[t_index];
			t_window->Destroy();
			delete t_window;
			m_windows[t_index] = nullptr;
			m_availableIds.push_back(t_index);
			break;
		}
	}

	void ImGuiLayer::OnUiRender()
	{
		bool dockSpaceOpen = true;

		Layer::OnUiRender();
		m_imGui->NewFrame();
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("DockSpace", &dockSpaceOpen, window_flags);
		ImGuiID t_dockSpaceID = ImGui::GetID("MainDockSpace");
		ImGuiDockNodeFlags t_dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGui::DockSpace(t_dockSpaceID, ImVec2(0.0f, 0.0f), t_dockspaceFlags);
		ImGui::PopStyleVar(2);
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::BeginMenu("Add Window"))
				{
					if (ImGui::MenuItem("Viewport"))
					{
						
					}
					if (ImGui::MenuItem("Scene Graph"))
					{
						SceneGraphUiWindow* t_window = new SceneGraphUiWindow(m_renderer,this, m_app->GetCurrentScene());
						t_window->SetName("Scene Graph");
						AddWindow(t_window);
					}
					if (ImGui::MenuItem("Inspector"))
					{
						InspectorUiWindow* t_window = new InspectorUiWindow(m_renderer, this);
						t_window->SetName("Inspector");
						AddWindow(t_window);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::End();
		

		ImGui::Begin("Viewport");
		m_imGui->DrawSceneAsImage();
		ImGui::End();

		for (UiWindow* t_window : m_windows)
		{
			if (t_window == nullptr)
				continue;
			t_window->UiDraw();	
		}

		m_imGui->Render();

	}

	void ImGuiLayer::Delete()
	{
		//delete every UiWindow
		for (UiWindow* t_window : m_windows)
		{
			t_window->Destroy();
			delete t_window;
		}
		if (m_imGui)
		{
			//m_imGui->Cleanup();
			delete m_imGui;
			m_imGui = nullptr;
		}
		Layer::OnDetach();
	}

	void ImGuiLayer::AddWindow(UiWindow* a_window)
	{
		if (a_window == nullptr)
			return;
		if (!m_availableIds.empty())
		{
			int id = m_availableIds.back();
			m_availableIds.pop_back();
			m_windows[id] = a_window;
			a_window->SetUid(static_cast<int>(id));
		}
		else
		{
			m_windows.push_back(a_window);
			a_window->Create();
			a_window->SetUid(static_cast<int>(m_windows.size()) - 1);
		}
	}
}
