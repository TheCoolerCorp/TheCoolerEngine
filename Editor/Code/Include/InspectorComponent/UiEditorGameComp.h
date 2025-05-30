#ifndef UIEDITORGAMECOMP_H
#define UIEDITORGAMECOMP_H

#include "InspectorWindow.h"
#include "../ImGuiLayer.h"
#include "Components/EditorGameComponent.h"
#include "GamePlay/Components/ReflectionComponents/ComponentRegistry.h"
#include "UiInspectorComponent.h"

namespace Editor::EditorLayer::Ui
{
	/*
	* UiEditorGameComponent is a base class for all editor game components that need to be displayed in the inspector.
	* It inherits from EditorGameComponent and implements the UiDraw function to display the component in the inspector.
	*/
	class UiEditorGameComponent : public InspectorComponent
	{
	public:
		UiEditorGameComponent(ImGuiLayer* a_layer, Engine::GamePlay::GameComponent* a_editorGameComponent, InspectorUiWindow* a_uiWindow)
			:InspectorComponent(a_layer), m_editorGameComponent(a_editorGameComponent)
		{
			if (a_uiWindow)
			{
				m_window = a_uiWindow;
				static_cast<GamePlay::EditorGameComponent*>(a_editorGameComponent)->SetInspectorWindow(a_uiWindow);
			}
			

		}

		~UiEditorGameComponent() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;
		UiComponentType GetType() override;
	protected:
		Engine::GamePlay::GameComponent* m_editorGameComponent = nullptr;
	};
}

#endif // UIEDITORGAMECOMP_H