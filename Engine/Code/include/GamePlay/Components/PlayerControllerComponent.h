#ifndef PLAYERCONTROLLERCOMPONENT_H
#define PLAYERCONTROLLERCOMPONENT_H

#include "EngineExport.h"
#include "GamePlay/Components/GameComponent.h"
#include "Math/vec2.h"

namespace Engine::Core::Window
{
	class IInputHandler;
}

namespace Engine::GamePlay
{
	
	class Scene;
	class TransformComponent;
	class RigidBodyComponent;

	class PlayerControllerComponent : public Engine::GamePlay::GameComponent
	{
	public:
		ENGINE_API PlayerControllerComponent();
		ENGINE_API ~PlayerControllerComponent() override;
		ENGINE_API ComponentType Create(int& a_outId) override;
		ENGINE_API void Destroy() override;
		ENGINE_API void Start() override;
		ENGINE_API void Update() override;
		ENGINE_API void SceneUpdate() override;
		ENGINE_API void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;
	
		ENGINE_API static ComponentType GetType() { return ComponentType::PLAYERCONTROLLER; }
		ENGINE_API static void RemoveComponent(int a_id);
		ENGINE_API static PlayerControllerComponent* GetComponent(int a_id);

		[[nodiscard]] ENGINE_API float GetMaxSpeed() const { return m_maxSpeed; }
		[[nodiscard]] ENGINE_API float GetRotationSpeed() const { return m_sensitivity; }
		[[nodiscard]] ENGINE_API float GetJumpForce() const { return m_jumpForce; }
		[[nodiscard]] ENGINE_API float GetMoveSpeed() const { return m_moveSpeed; }
		ENGINE_API void SetMaxSpeed(float a_maxSpeed) { m_maxSpeed = a_maxSpeed; }
		ENGINE_API void SetRotationSpeed(float a_rotationSpeed) { m_sensitivity = a_rotationSpeed; }
		ENGINE_API void SetJumpForce(float a_jumpForce) { m_jumpForce = a_jumpForce; }
		ENGINE_API void SetMoveSpeed(float a_moveSpeed) { m_moveSpeed = a_moveSpeed; }

		ENGINE_API void OverrideRotateTransform(int a_transformId) { m_transformRotateComponentId = a_transformId; }

		ENGINE_API int GetRotateTransformId() { return m_transformRotateComponentId; }
	private:
		Math::vec2 m_oldMousePos = Math::vec2(0.f, 0.f);

		int m_transformComponentId = -1;
		int m_transformRotateComponentId = -1;
		int m_rigidBodyComponentId = -1;

		float m_maxSpeed = 3;
		float m_sensitivity = 1.f;
		float m_jumpForce = 10.f;
		float m_moveSpeed = 30.f;

		float m_maxUpAngle = 85.f;
		float m_maxDownAngle = 85.f;

		RigidBodyComponent* GetRigidbody();
		TransformComponent* GetTransform();
		TransformComponent* GetTransformRotate();

		float ClampAngle(float a_angle);
	};
}


#endif // !PLAYERCONTROLLERCOMPO NENT_H