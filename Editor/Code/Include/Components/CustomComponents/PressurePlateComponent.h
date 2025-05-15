#ifndef PRESSUREPLATECOMPONENT_H
#define PRESSUREPLATECOMPONENT_H

#include "../EditorGameComponent.h"
#include "../ComponentRegistry.h"

namespace Editor::GamePlay
{
	class PressurePlateComponent : public EditorGameComponent
	{
	public:
		PressurePlateComponent() = default;
		~PressurePlateComponent() override = default;

		void DrawUI() override;

		void Start() override;
		void Update() override;
		void SceneUpdate() override;
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;

		static std::string StaticTypeName() { return "PressurePlateComponent"; }
		std::string GetTypeName() const override { return StaticTypeName(); }
	};
}

#endif // !PRESSUREPLATECOMPONENT_H