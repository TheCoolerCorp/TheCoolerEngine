#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include <vector>

#include "imgui.h"
#include "Layer.h"
#include "RHIImGui.h"
#include "GamePlay/Others/GameObject.h"



namespace Editor::EditorLayer::Ui
{
	class UiWindow;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(Engine::Core::Renderer* a_renderer, Core::Application* a_app, const std::string& a_name = "ImGui Layer"): Layer(a_renderer, a_app, a_name)
		{}

		~ImGuiLayer() override = default;
		void OnAttach(Engine::Core::Window::IWindow* a_window) override;
		void OnDetach() override;
		void OnUpdate(float a_deltaTime) override;
		void OnUiRender() override;
		void Delete() override;

		void AddWindow(UiWindow* a_window);

		void SetSelectedGameObject(Engine::GamePlay::GameObject* a_object) { m_selectedGameObject = a_object; }
		Engine::GamePlay::GameObject* GetSelectedGameObject() const { return m_selectedGameObject; }

	private:
		Engine::GamePlay::GameObject* m_selectedGameObject = nullptr;
		RHIImGui* m_imGui = nullptr;
		std::vector<UiWindow*> m_windows;
		std::vector<int> m_availableIds;

		void SetupImGuiStyle();
		ImVec4 ToSrgb(ImVec4 rgba);
	};
}

#endif