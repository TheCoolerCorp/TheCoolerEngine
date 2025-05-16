#ifndef EDITORGAMECOMPONENT_H
#define EDITORGAMECOMPONENT_H

#include "InspectorWindow.h"
#include "GamePlay/Components/Component.h"
#include "GamePlay/Components/GameComponent.h"
#include "Core/Window/IInputHandler.h"
#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Others/Scene.h"

namespace Editor::GamePlay
{
	class EditorGameComponent : public Engine::GamePlay::GameComponent
	{
	public:
		EditorGameComponent() = default;
		~EditorGameComponent() override = default;

		void Create(int& a_outId) override
		{
			a_outId = Engine::GamePlay::ServiceLocator::GetGameComponentSystem()->AddComponent(this);
			SetId(a_outId);
		}
		void Destroy() override {}

		std::string GetTypeName() const override { return ""; }


		// Editor specific functions
		void DrawUI() override {}

		// GameComponent functions
		void Start() override {}
		void Update(float a_deltatime) override {}
		void SceneUpdate() override {}
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override {}

		void SetInspectorWindow(EditorLayer::Ui::InspectorUiWindow* a_window)
		{
			m_uiWindow = a_window;
		}
	protected:
		EditorLayer::Ui::InspectorUiWindow* m_uiWindow = nullptr;

		Engine::GamePlay::GameObject* GetGameObject() const
		{
			if (m_scene)
			{
				return m_scene->GetGameObject(m_gameObjectId);
			}
			return nullptr;
		}
	};

	/*
	* Editor-Integrated GameComponent.
	* Offers further functionality like UI draw and automatic registration for easier usage in the ui.
	*/
}
#endif // !EDITORGAMECOMPONENT_H