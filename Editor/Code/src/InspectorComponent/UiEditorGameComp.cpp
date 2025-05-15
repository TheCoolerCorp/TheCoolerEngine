#include "../../Include/InspectorComponent/UiEditorGameComp.h"

Editor::EditorLayer::Ui::UiEditorGameComponent::~UiEditorGameComponent()
{
}

void Editor::EditorLayer::Ui::UiEditorGameComponent::Create()
{
}

void Editor::EditorLayer::Ui::UiEditorGameComponent::UiDraw()
{
	m_editorGameComponent->DrawUI();
}

void Editor::EditorLayer::Ui::UiEditorGameComponent::Destroy()
{
	m_editorGameComponent = nullptr;
}

Editor::EditorLayer::Ui::UiComponentType Editor::EditorLayer::Ui::UiEditorGameComponent::GetType()
{
	return UiComponentType::EDITORGAMECOMPONENT;
}
