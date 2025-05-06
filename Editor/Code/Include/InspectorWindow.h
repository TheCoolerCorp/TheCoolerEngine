#ifndef INSPECTORUIWINDOW_H
#define INSPECTORUIWINDOW_H

#include "UiWindow.h"
#include "GamePlay/Others/GameObject.h"



namespace Editor::EditorLayer::Ui
{
	enum UiComponentType
	{
		TRANSFORM,
		MESH,
		RIGIDBODY,
		LIGHT
	};

	class InspectorComponent;

	class InspectorUiWindow : public UiWindow
	{
	public:
		InspectorUiWindow(Engine::Core::Renderer* a_renderer, ImGuiLayer* a_layer)
			: UiWindow(a_renderer, a_layer)
		{
			m_name = "Inspector";
		}
		~InspectorUiWindow() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;
		void NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object) override;

		Engine::GamePlay::GameObject* GetSelectedObject() { return m_selectedObject; }

		bool HasComponentOfType(UiComponentType a_type);
	private:
		

		bool m_locked = false;
		Engine::GamePlay::GameObject* m_selectedObject = nullptr;
		std::vector<InspectorComponent*> m_objectComponents;

		void RefreshSelectedObject();
		void RefreshCurrentObject();
		bool IsObjectOutOfDate() const;
		void AddComponent(InspectorComponent* a_component);
		void ClearComponents();

		//ui helpers
		void CreateNameTextField();
		void DrawComponentAddWindow();
	};
}

#endif	