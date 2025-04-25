#ifndef INSPECTORUIWINDOW_H
#define INSPECTORUIWINDOW_H

#include "UiWindow.h"
#include "GamePlay/Others/GameObject.h"



namespace Editor::EditorLayer::Ui
{
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


		
	private:
		

		bool m_locked = false;
		Engine::GamePlay::GameObject* m_selectedObject = nullptr;
		std::vector<InspectorComponent*> m_objectComponents;

		void RefreshSelectedObject();
		bool IsObjectOutOfDate() const;
		void AddComponent(InspectorComponent* a_component);
		void ClearComponents();

		//ui helpers
		void CreateNameTextField();
	};
}

#endif	