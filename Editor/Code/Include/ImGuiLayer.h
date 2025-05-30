#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include <vector>

#include "imgui.h"
#include "Layer.h"
#include "RHIImGui.h"
#include "GamePlay/Others/GameObject.h"

//IMGUIZMO
#include "ImGuizmo.h"

namespace Engine::Core
{
	namespace Window
	{
		class IInputHandler;
	}
}
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
		//some stuff, specifically setting up events, is not ready at the time imgui is created,
		//as such we wait for the first frame update to do these
		void Setup();
		void OnDetach() override;
		void OnUpdate(float a_deltaTime) override;
		void OnUiRender() override;
		void OnProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;
		void OnBeginPlay();
		void Delete() override;

		void AddWindow(UiWindow* a_window);

		void SetSelectedGameObject(Engine::GamePlay::GameObject* a_object) { m_selectedGameObject = a_object; }
		Engine::GamePlay::GameObject* GetSelectedGameObject() const { return m_selectedGameObject; }

		void DeselectObject();
		void NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object);

		void UiDrawItemAddMenu();

		void MouseCursorCaptured(bool a_captured);

		/*
		 * Since ImGuizmo functionality is so intertwined with ImGui, all functionality related to it
		 * Will be made in the ImGuiLayer class. All ImGuizmo-related functions will start with "Gizmo" for distinction
		 */
		 
		void GizmoBeginFrame();
		void GizmoMainDraw();

		[[nodiscard]] ImGuizmo::OPERATION GizmoGetCurrentOperation() const { return m_currentGizmoOperation; }
		void GizmoSetCurrentOperation(ImGuizmo::OPERATION a_operation) { m_currentGizmoOperation = a_operation; }
		[[nodiscard]] ImGuizmo::MODE GizmoGetCurrentMode() const { return m_currentGizmoMode; }
		void GizmoSetCurrentMode(ImGuizmo::MODE a_mode) { m_currentGizmoMode = a_mode; }


	private:
		bool m_setup = false;
		bool m_mouseCaptured = false;
		bool m_previousCaptueState = false;
		bool m_captureOverwritten = false;

		//the selected game object, used for gizmo manipulation and by the inspector window
		Engine::GamePlay::GameObject* m_selectedGameObject = nullptr;

		//an RHI for imgui is needed because certain functions and behaviours are specific to certain implementations, like vulkan or glfw
		RHIImGui* m_imGui = nullptr;

		std::vector<UiWindow*> m_windows;
		std::vector<int> m_availableIds;

		//ImGuiZmo
		ImGuizmo::OPERATION m_currentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE m_currentGizmoMode = ImGuizmo::LOCAL;

		void SetupImGuiStyle();
		ImVec4 ToSrgb(ImVec4 rgba);

		//ui helpers
		void UiSetupDockspace();
		void UiDrawMenuBar();
		void UiDrawViewport();
		void UiDrawWindows();

	};
}

#endif