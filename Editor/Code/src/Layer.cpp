#include "../Include/Layer.h"

#include "Application.h"

Engine::GamePlay::Scene* Editor::EditorLayer::Layer::GetScene()
{
	return m_app->GetCurrentScene();
}