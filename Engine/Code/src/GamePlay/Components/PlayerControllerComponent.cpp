#include "GamePlay/Components/PlayerControllerComponent.h"
#include "GamePlay/ServiceLocator.h"
#include "GamePlay/Others/Scene.h"

#include "GamePlay/Components/TransformComponent.h"
#include "GamePlay/Components/RigidBodyComponent.h"
#include "Core/Window/IInputHandler.h"
#include "GamePlay/Systems/GameComponentSystem.h"
#include "Math/TheCoolerMath.h"

#include <meta/factory.hpp>
#include <meta/meta.hpp>

namespace Engine::GamePlay
{
	PlayerControllerComponent::PlayerControllerComponent()
	{
	}
	PlayerControllerComponent::~PlayerControllerComponent()
	{
	}
	void PlayerControllerComponent::Create(int& a_outId)
	{
		a_outId = ServiceLocator::GetGameComponentSystem()->AddComponent(this);
		SetId(a_outId);
	}
	void PlayerControllerComponent::Destroy()
	{
	}

	void PlayerControllerComponent::Start()
	{
		GameObject* t_gameObject = m_scene->GetGameObject(m_gameObjectId);
		m_transformComponentId = t_gameObject->GetComponentID<TransformComponent>();
		m_rigidBodyComponentId = t_gameObject->GetComponentID<RigidBodyComponent>();
		if (!m_rigidBodyComponentId)
		{
			LOG_ERROR("Player Controller: no rigidbody assigned to gameobject!");
		}
		CaptureCursor();
	}

	void PlayerControllerComponent::Update()
	{
	}
	void PlayerControllerComponent::SceneUpdate()
	{
	}

	void PlayerControllerComponent::ProcessInputs(Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
	{
		TransformComponent* t_transform = GetTransform();
		TransformComponent* t_transformRotate = GetTransformRotate();
		RigidBodyComponent* t_rigidBody = GetRigidbody();
		Math::vec3 t_movement = Math::vec3(0,0,0);
		if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_W))
		{
			t_movement += t_transformRotate->GetRight();
		}
		if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_S))
		{
			t_movement -= t_transformRotate->GetRight();
		}
		if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_A))
		{
			t_movement += t_transformRotate->GetForward();
		}
		if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_D))
		{
			t_movement -= t_transformRotate->GetForward();
		}
		if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_SPACE))
		{
			if (t_rigidBody->GetVelocity().y < 0.01 && t_rigidBody->IsGrounded(t_transform->GetTransform()->GetGlobalPosition(), t_transform->GetTransform()->GetGlobalRotation())) //a rather simplistic solution for jumping,, for now
			{
				t_rigidBody->AddImpulse(t_transform->GetUp() * m_jumpForce);
			}
		}

		//mouse movement
		const Math::vec2 t_currentMousePos = a_inputHandler->GetCursorPosition();
		Math::vec2 t_deltaMousePos = t_currentMousePos - m_oldMousePos;

		float t_pitch = -t_deltaMousePos.y * m_sensitivity * a_deltaTime;
		float t_yaw = -t_deltaMousePos.x * m_sensitivity * a_deltaTime;

		Math::vec3 t_rotation = t_transformRotate->GetTransform()->GetEulerAngles();
		t_rotation.x = ClampAngle(t_rotation.x+t_pitch);
		t_rotation.y += t_yaw;
		t_transformRotate->GetTransform()->SetRotation(t_rotation);
		m_oldMousePos = a_inputHandler->GetCursorPosition();

		if (Math::vec3::Norm(t_movement) >= 0.003 || Math::vec3::Norm(t_movement) <= -0.003)
		{
			t_movement = Math::vec3::Normalize(t_movement);
			t_movement = t_movement * 100 * m_moveSpeed * a_deltaTime;
			t_rigidBody->AddForce(Math::vec3(t_movement.z, 0, t_movement.x));
		}
		Math::vec3 t_velocity = t_rigidBody->GetVelocity();
		float t_norm = Math::vec3::Norm(Math::vec3(t_velocity.x, 0, t_velocity.z));
		if (t_norm > m_maxSpeed)
		{
			t_velocity = Math::vec3(t_velocity.x, 0, t_velocity.z);
			t_velocity = Math::vec3::Normalize(t_velocity) * m_maxSpeed;
			t_velocity.y = t_rigidBody->GetVelocity().y;
			t_rigidBody->SetLinearVelocity(t_velocity);
		}
	}

	void PlayerControllerComponent::Register()
	{
		constexpr std::hash<std::string_view> t_hash{};

		meta::reflect<PlayerControllerData>(t_hash("PlayerControllerData"))
			.data<&PlayerControllerData::m_maxSpeed>(t_hash("maxSpeed"))
			.data<&PlayerControllerData::m_sensitivity>(t_hash("sensitivity"))
			.data<&PlayerControllerData::m_jumpForce>(t_hash("jumpForce"))
			.data<&PlayerControllerData::m_moveSpeed>(t_hash("moveSpeed"))
			.data<&PlayerControllerData::m_maxUpAngle>(t_hash("maxUpAngle"))
			.data<&PlayerControllerData::m_maxDownAngle>(t_hash("maxDownAngle"))
			.data<&PlayerControllerData::m_transformRotateComponentId>(t_hash("transformRotateComponentId"));

		meta::reflect<PlayerControllerComponent>(t_hash("PlayerControllerComponent"))
			.data<&PlayerControllerComponent::Set, &PlayerControllerComponent::GetData>(t_hash("PlayerController"));
	}

	void PlayerControllerComponent::Set(const PlayerControllerData& a_data)
	{
		m_maxSpeed = a_data.m_maxSpeed;
		m_sensitivity = a_data.m_sensitivity;
		m_jumpForce = a_data.m_jumpForce;
		m_moveSpeed = a_data.m_moveSpeed;
		m_maxUpAngle = a_data.m_maxUpAngle;
		m_maxDownAngle = a_data.m_maxDownAngle;
		m_transformRotateComponentId = a_data.m_transformRotateComponentId;
	}

	PlayerControllerData PlayerControllerComponent::GetData() const
	{
		return { .m_maxSpeed= m_maxSpeed, .m_sensitivity= m_sensitivity, .m_jumpForce= m_jumpForce, .m_moveSpeed=
			m_moveSpeed,
			.m_maxUpAngle= m_maxUpAngle, .m_maxDownAngle= m_maxDownAngle, .m_transformRotateComponentId=
			m_transformRotateComponentId
		};
	}

	void PlayerControllerComponent::RemoveComponent(int a_id)
	{
		ServiceLocator::GetGameComponentSystem()->RemoveComponent(a_id);
	}

	PlayerControllerComponent* PlayerControllerComponent::GetComponent(int a_id)
	{
		return dynamic_cast<PlayerControllerComponent*>(ServiceLocator::GetGameComponentSystem()->GetComponent(a_id));
	}

	/**
	 * utility function to get a pointer to the rigidbodycomponent from the id
	 */
	RigidBodyComponent* PlayerControllerComponent::GetRigidbody()
	{
		RigidBodyComponent* t_rigidBody = ServiceLocator::GetPhysicsSystem()->GetComponent(m_rigidBodyComponentId);
		if (t_rigidBody == nullptr)
		{
			return nullptr;
		}
		return t_rigidBody;
	}

	/**
	 * utility function to get a pointer to the transformcomponent from the id
	 */
	TransformComponent* PlayerControllerComponent::GetTransform()
	{
		TransformComponent* t_transform = ServiceLocator::GetTransformSystem()->GetComponent(m_transformComponentId);
		if (t_transform == nullptr)
		{
			return nullptr;
		}
		return t_transform;
	}

	TransformComponent* PlayerControllerComponent::GetTransformRotate()
	{
		if (m_transformRotateComponentId == -1)
		{
			return GetTransform();
		}
		TransformComponent* t_transform = ServiceLocator::GetTransformSystem()->GetComponent(m_transformRotateComponentId);
		if (t_transform == nullptr)
		{
			return nullptr;
		}
		return t_transform;
	}

	float PlayerControllerComponent::ClampAngle(float a_angle)
	{
		a_angle = std::min(a_angle, m_maxUpAngle * (Math::PI / 180.0f));
		a_angle = std::max(a_angle, -m_maxDownAngle * (Math::PI / 180.0f));
		return a_angle;
	}
}
