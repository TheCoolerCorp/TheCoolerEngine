#ifndef INSPECTORUIWINDOW_H
#define INSPECTORUIWINDOW_H

#include "UiWindow.h"
#include "GamePlay/Others/GameObject.h"

namespace Editor::EditorLayer::Ui
{
	class InspectorUiWindow : public UiWindow
	{
	public:
		InspectorUiWindow(Engine::Core::Renderer* a_renderer, ImGuiLayer* a_layer)
			: UiWindow(a_renderer, a_layer)
		{
			m_name = "Inspector";
		}
		~InspectorUiWindow() override = default;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

	private:
		bool locked = false;
		Engine::GamePlay::GameObject* m_selectedObject = nullptr;
	};
}

#endif