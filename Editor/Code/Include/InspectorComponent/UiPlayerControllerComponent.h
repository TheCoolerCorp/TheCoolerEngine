#ifndef UIPLAYERCONTROLLERCOMPONENT_H
#define UIPLAYERCONTROLLERCOMPONENT_H

#include "UiInspectorComponent.h"
#include "GamePlay/Components/PlayerControllerComponent.h"


namespace Editor::EditorLayer::Ui
{
	class UiPlayerControllerComponent : public InspectorComponent
	{
	public:
		UiPlayerControllerComponent(ImGuiLayer* a_layer, Engine::GamePlay::PlayerControllerComponent* a_playerController)
			: InspectorComponent(a_layer), m_playerController(a_playerController)
		{
		}

		~UiPlayerControllerComponent() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

		UiComponentType GetType() override
		{
			return UiComponentType::PLAYERCONTROLLER;
		}
	private:
		Engine::GamePlay::PlayerControllerComponent* m_playerController = nullptr;

		void AddDragDropRotateTransformOverride();
	};
}

#endif // UIPLAYERCONTROLLERCOMPONENT_H