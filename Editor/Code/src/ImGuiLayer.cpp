#include "../Inlude/ImGuiLayer.h"
#include "../Inlude/VulkanImGui.h"
#include "Core/Renderer/Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

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

	void ImGuiLayer::Delete()
	{
		if (m_imGui)
		{
			//m_imGui->Cleanup();
			delete m_imGui;
			m_imGui = nullptr;
		}

		Layer::OnDetach();
	}
}
