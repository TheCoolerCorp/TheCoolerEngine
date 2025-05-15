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

		//unfortunately these two static functions will need to be implemented for every EditorGameComponent
		static void RemoveComponent(int a_id);
		static PressurePlateComponent* GetComponent(int a_id);

		static std::string StaticTypeName() { return "PressurePlateComponent"; }
		std::string GetTypeName() const override { return StaticTypeName(); }

		void OnColisionEnter(Engine::GamePlay::RigidBodyComponent* component);
		void OnColisionExit(Engine::GamePlay::RigidBodyComponent* component);

		CoolerEvent<bool>& GetPressurePlatePressedEvent() { return m_pressurePlatePressedEvent; }
	private:
		CoolerEvent<bool> m_pressurePlatePressedEvent;
		void UiAddCollisionTargetDragDropTarget();

		int m_transformComponentId = -1;
		int m_collisionListenerGameObject = -1;
	};
}

#endif // !PRESSUREPLATECOMPONENT_H