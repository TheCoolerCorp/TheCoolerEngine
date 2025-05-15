#ifndef EDITORGAMECOMPONENT_H
#define EDITORGAMECOMPONENT_H

#include "InspectorWindow.h"
#include "GamePlay/Components/Component.h"
#include "GamePlay/Components/GameComponent.h"
#include "Core/Window/IInputHandler.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Others/Scene.h"

namespace Editor::GamePlay
{
	/*
	* Editor-Integrated GameComponent.
	* Offers further functionality like UI draw and automatic registration for easier usage in the ui.
	*/
	class EditorGameComponent : public Engine::GamePlay::GameComponent
	{
	public:
		EditorGameComponent() = default;
		~EditorGameComponent() override = default;

		virtual std::string GetTypeName() const = 0;

		// Editor specific functions
		virtual void DrawUI() = 0;

		
		// GameComponent functions
		void Create(int& a_outId) override
		{
			a_outId = Engine::GamePlay::ServiceLocator::GetGameComponentSystem()->AddComponent(this);
			SetId(a_outId);
		};
		void Destroy() override {}

		
		void Start() override {}
		void Update() override {}
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
}
#endif // !EDITORGAMECOMPONENT_H