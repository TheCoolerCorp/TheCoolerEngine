#ifndef PRESSUREPLATECOMPONENT_H
#define PRESSUREPLATECOMPONENT_H

#include "../EditorGameComponent.h"

namespace Editor::GamePlay
{
	class PressurePlateComponent : public EditorGameComponent
	{
	public:
		PressurePlateComponent() = default;
		~PressurePlateComponent() override = default;

		void DrawUI() override;

		void Start() override;
		void Update(float a_deltatime) override;
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

		bool m_pressed = false;

		int m_transformComponentId = -1;
		int m_collisionListenerGameObjectId = -1;
		int m_doorGameObjectId = -1;

		//the position of the door when the pressure plate is pressed
		Engine::Math::vec3 m_doorActivePos = { 0.f, 0.f, 0.f };
		Engine::Math::vec3 m_doorInactivePos = { 0.f, 0.f, 0.f };

		void MoveTowards(Engine::Math::vec3& a_pos, const Engine::Math::vec3 a_targetPos, float a_deltatime, float a_maxSpeed);
	};
}

#endif // !PRESSUREPLATECOMPONENT_H