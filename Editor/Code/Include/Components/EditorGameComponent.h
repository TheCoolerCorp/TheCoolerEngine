#ifndef EDITORGAMECOMPONENT_H
#define EDITORGAMECOMPONENT_H

#include "GamePlay/Components/Component.h"
#include "GamePlay/Components/GameComponent.h"
#include "Core/Window/IInputHandler.h"

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
		void Create(int& a_outId) override {}
		void Destroy() override {}

		void Start() override {}
		void Update() override {}
		void SceneUpdate() override {}
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override {}
	};
}
#endif // !EDITORGAMECOMPONENT_H