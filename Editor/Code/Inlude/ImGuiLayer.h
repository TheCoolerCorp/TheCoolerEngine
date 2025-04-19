#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Layer.h"
#include "RHIImGui.h"
namespace Editor::EditorLayer::Ui
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(Engine::Core::Renderer* a_renderer, const std::string& a_name = "ImGui Layer"): Layer(a_renderer)
		{
			m_name = a_name;
			m_renderer = a_renderer;
		}

		~ImGuiLayer() override = default;
		void OnAttach(Engine::Core::Window::IWindow* a_window) override;
		void OnDetach() override;
		void OnUpdate(float a_deltaTime) override;
		void OnUiRender() override;
		void Delete() override;

	private:
		RHIImGui* m_imGui = nullptr;
	};
}

#endif