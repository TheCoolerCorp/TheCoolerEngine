#include <algorithm>

#include "../Include/InspectorComponent/UiRigidbodyComponent.h"

#include "imgui.h"
#include "GamePlay/Components/RigidBodyComponent.h"
#include "GamePlay/Others/GameObject.h"

Editor::EditorLayer::Ui::UiRigidbodyComponent::~UiRigidbodyComponent()
{
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::Create()
{
}

/**
 * Display the RigidbodyComponent properties in the inspector.
 * Currently displays position, rotation, scale/radius/halfheight, static/dynamic/kinematic, is trigger, debug and capsule/sphere/box
 * Does not support changing most properties yet
 */
void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiDraw()
{
	Engine::Math::vec3 t_pos = m_rigidBody->GetPos();
	Engine::Math::vec3 t_rot = Engine::Math::quat::ToEulerAngles(m_rigidBody->GetRot());

	float t_fPos[4] = { t_pos.x, t_pos.y, t_pos.z, 0.f };
	float t_fRot[4] = { t_rot.x, t_rot.y, t_rot.z, 0.f };

	bool t_isActive = m_rigidBody->GetBody()->IsActive();
	bool t_isTrigger = m_rigidBody->IsTrigger();
	bool t_isDebug = m_rigidBody->GetDebug();

	ImGui::SeparatorText("Rigidbody Component");
	ImGui::SameLine();
	if (ImGui::Button(("Remove##Rigidbody"+std::to_string(m_uid)).c_str()))
	{
		m_window->GetSelectedObject()->RemoveComponent<Engine::GamePlay::RigidBodyComponent>();
		m_window->MarkOutOfDate();
		return;
	}

	ImGui::Text("Active: ");
	ImGui::SameLine();
	if(ImGui::Checkbox(("##Active"+ std::to_string(m_uid)).c_str(), &t_isActive))
	{
		m_rigidBody->SetActive(t_isActive);
	}
	ImGui::SameLine();
	ImGui::Text("Is Trigger: ");
	ImGui::SameLine();
	if (ImGui::Checkbox(("##IsTrigger" + std::to_string(m_uid)).c_str(), &t_isTrigger))
	{
		m_rigidBody->SetIsTrigger(t_isTrigger);
	}
	ImGui::SameLine();
	ImGui::Text("Debug: ");
	ImGui::SameLine();
	if (ImGui::Checkbox(("##Debug" + std::to_string(m_uid)).c_str(), &t_isDebug))
	{
		m_rigidBody->SetDebug(t_isDebug);
	}
	//Show collider values
	UiShowColliderInfo();
	UiShowBodyType();
	UiShowCollisionLayer();
	//Show axis lock checkboxes
	UiShowAxisLockCheckboxes();
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::Destroy()
{
	//nothing to destroy
}

/**
 * Allows the user to lock/unlock the rotational axes of the rigidbody.
 */
void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiShowAxisLockCheckboxes()
{
	ImGui::Text("Lock Rotation: ");
	ImGui::SameLine();
	bool t_lockX = m_rigidBody->GetBody()->IsRotLockedX();
	if (ImGui::Checkbox(("X##" + std::to_string(m_uid)).c_str(), &t_lockX))
	{
		if (t_lockX)
			m_rigidBody->GetBody()->LockRotation('x');
		else
			m_rigidBody->GetBody()->UnlockRotation('x');
	}
	ImGui::SameLine();
	bool t_lockY = m_rigidBody->GetBody()->IsRotLockedY();
	if (ImGui::Checkbox(("Y##" + std::to_string(m_uid)).c_str(), &t_lockY))
	{
		if (t_lockY)
			m_rigidBody->GetBody()->LockRotation('y');
		else
			m_rigidBody->GetBody()->UnlockRotation('y');
	}
	ImGui::SameLine();
	bool t_lockZ = m_rigidBody->GetBody()->IsRotLockedZ();
	if (ImGui::Checkbox(("Z##" + std::to_string(m_uid)).c_str(), &t_lockZ))
	{
		if (t_lockZ)
			m_rigidBody->GetBody()->LockRotation('z');
		else
			m_rigidBody->GetBody()->UnlockRotation('z');
	}
}

/**
 * Shows different info in the ui based on the collider type.
 */
void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiShowColliderInfo()
{
	ImGui::Text("Collider Type: ");
	ImGui::SameLine();
	switch (m_rigidBody->GetColliderType())
	{
	case Engine::Physics::ColliderType::BOX:
	{
		UiDrawColliderTypeCombo("Box");
		Engine::Math::vec3 t_scale = m_rigidBody->GetBody()->GetScale();
		float t_fScale[4] = { t_scale.x, t_scale.y, t_scale.z, 0.f };

		if (ImGui::DragFloat3(("Scale##" + std::to_string(m_uid)).c_str(), t_fScale, 0.1f,0.01f, +FLT_MAX))
		{
			for (int i = 0; i < 4; i++)
			{
				t_fScale[i] = std::max(t_fScale[i], 0.1f);
			}
			m_rigidBody->GetBody()->SetScale(Engine::Math::vec3(t_fScale[0], t_fScale[1], t_fScale[2]));
		}
		break;
	}
	case Engine::Physics::ColliderType::SPHERE:
	{
		UiDrawColliderTypeCombo("Sphere");
		float t_radius = m_rigidBody->GetBody()->GetRadius();
		if (ImGui::DragFloat(("Radius##" + std::to_string(m_uid)).c_str(), &t_radius, 0.1f, 0.001f, +FLT_MAX))
		{
			t_radius = std::clamp(t_radius, 0.01f, +FLT_MAX);
			m_rigidBody->GetBody()->SetRadius(t_radius);
		}
		break;
	}
	case Engine::Physics::ColliderType::CAPSULE:
	{
		UiDrawColliderTypeCombo("Capsule");
		float t_radius = m_rigidBody->GetBody()->GetRadius();
		float t_halfHeight = m_rigidBody->GetBody()->GetHalfHeight();
		if (ImGui::DragFloat(("HalfHeight##" + std::to_string(m_uid)).c_str(), &t_halfHeight, 0.1f, 0.001f, +FLT_MAX))
		{
			t_halfHeight = std::clamp(t_halfHeight, 0.01f, +FLT_MAX);
			m_rigidBody->GetBody()->SetHalfHeight(t_halfHeight);
		}
		if (ImGui::DragFloat(("Radius##" + std::to_string(m_uid)).c_str(), &t_radius, 0.1f, 0.001f, +FLT_MAX))
		{
			t_radius = std::clamp(t_radius, 0.01f, +FLT_MAX);
			m_rigidBody->GetBody()->SetRadius(t_radius);
		}
		break;
	}
	}
	float t_mass = m_rigidBody->GetBody()->GetMass();
	if (ImGui::DragFloat(("Mass##" + std::to_string(m_uid)).c_str(), &t_mass, 0.1f, 0.001f, +FLT_MAX))
	{
		m_rigidBody->GetBody()->SetMass(t_mass);
	}
	float t_friction = m_rigidBody->GetBody()->GetFriction();
	if (ImGui::DragFloat(("Friction##" + std::to_string(m_uid)).c_str(), &t_friction, 0.1f, 0.001f, +FLT_MAX))
	{
		t_friction = std::clamp(t_friction, 0.f, +FLT_MAX);
		m_rigidBody->GetBody()->SetFriction(t_friction);
	}
	float t_restitution = m_rigidBody->GetBody()->GetRestitution();
	if (ImGui::DragFloat(("Restitution##" + std::to_string(m_uid)).c_str(), &t_restitution, 0.1f, 0.001f, 1))
	{
		t_restitution = std::clamp(t_restitution, 0.f, 1.f);
		m_rigidBody->GetBody()->SetRestitution(t_restitution);
	}
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiDrawColliderTypeCombo(const std::string& a_activeComponent)
{
	if (ImGui::BeginCombo(("##"+a_activeComponent + std::to_string(m_uid)).c_str(), a_activeComponent.c_str()))
	{
		if (m_rigidBody->GetColliderType() != Engine::Physics::ColliderType::BOX)
		{
			if (ImGui::Selectable("Box"))
			{
				m_rigidBody->SetColliderType(Engine::Physics::ColliderType::BOX);
			}
		}
		if (m_rigidBody->GetColliderType() != Engine::Physics::ColliderType::SPHERE)
		{
			if (ImGui::Selectable("Sphere"))
			{
				m_rigidBody->SetColliderType(Engine::Physics::ColliderType::SPHERE);
			}
		}
		if (m_rigidBody->GetColliderType() != Engine::Physics::ColliderType::CAPSULE)
		{
			if (ImGui::Selectable("Capsule"))
			{
				m_rigidBody->SetColliderType(Engine::Physics::ColliderType::CAPSULE);
			}
		}
		ImGui::EndCombo();
	}
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiShowBodyType()
{
	switch (m_rigidBody->GetBody()->GetBodyType())
	{
	case Engine::Physics::BodyType::STATIC:
		UiDrawBodyTypeCombo("Static");
		break;
	case Engine::Physics::BodyType::KINEMATIC:
		UiDrawBodyTypeCombo("Kinematic");
		break;
	case Engine::Physics::BodyType::DYNAMIC:
		UiDrawBodyTypeCombo("Dynamic");
		break;
	}
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiDrawBodyTypeCombo(std::string a_activeComponent)
{
	if (ImGui::BeginCombo((a_activeComponent + "##" + std::to_string(m_uid)).c_str(), a_activeComponent.c_str()))
	{
		if (m_rigidBody->GetBody()->GetBodyType() != Engine::Physics::BodyType::STATIC)
		{
			if (ImGui::Selectable("Static"))
			{
				m_rigidBody->GetBody()->SetBodyType(Engine::Physics::BodyType::STATIC);
			}
		}
		if (m_rigidBody->GetBody()->GetBodyType() != Engine::Physics::BodyType::KINEMATIC)
		{
			if (ImGui::Selectable("Kinematic"))
			{
				m_rigidBody->GetBody()->SetBodyType(Engine::Physics::BodyType::KINEMATIC);
			}
		}
		if (m_rigidBody->GetBody()->GetBodyType() != Engine::Physics::BodyType::DYNAMIC)
		{
			if (ImGui::Selectable("Dynamic"))
			{
				m_rigidBody->GetBody()->SetBodyType(Engine::Physics::BodyType::DYNAMIC);
			}
		}
		ImGui::EndCombo();
	}
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiShowCollisionLayer()
{
	switch (m_rigidBody->GetBody()->GetLayer())
	{
	case Engine::Physics::CollisionLayer::NON_MOVING:
		UiDrawCollisionLayerCombo("Non-Moving");
		break;
	case Engine::Physics::CollisionLayer::MOVING:
		UiDrawCollisionLayerCombo("Moving");
		break;
	case Engine::Physics::CollisionLayer::TRIGGER:
		UiDrawCollisionLayerCombo("Trigger");
		break;
	case Engine::Physics::CollisionLayer::DISABLED:
		UiDrawCollisionLayerCombo("Disabled");
		break;
	}
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiDrawCollisionLayerCombo(std::string a_activeComponent)
{
	if (ImGui::BeginCombo((a_activeComponent + "##" + std::to_string(m_uid)).c_str(), a_activeComponent.c_str()))
	{
		if (m_rigidBody->GetBody()->GetLayer() != Engine::Physics::CollisionLayer::NON_MOVING)
		{
			if (ImGui::Selectable("Non-Moving"))
			{
				m_rigidBody->GetBody()->SetObjectLayer(Engine::Physics::CollisionLayer::NON_MOVING);
			}
		}
		if (m_rigidBody->GetBody()->GetLayer() != Engine::Physics::CollisionLayer::MOVING)
		{
			if (ImGui::Selectable("Moving"))
			{
				m_rigidBody->GetBody()->SetObjectLayer(Engine::Physics::CollisionLayer::MOVING);
			}
		}
		if (m_rigidBody->GetBody()->GetLayer() != Engine::Physics::CollisionLayer::TRIGGER)
		{
			if (ImGui::Selectable("Trigger"))
			{
				m_rigidBody->GetBody()->SetObjectLayer(Engine::Physics::CollisionLayer::TRIGGER);
			}
		}
		if (m_rigidBody->GetBody()->GetLayer() != Engine::Physics::CollisionLayer::DISABLED)
		{
			if (ImGui::Selectable("Disabled"))
			{
				m_rigidBody->GetBody()->SetObjectLayer(Engine::Physics::CollisionLayer::DISABLED);
			}
		}

		ImGui::EndCombo();
	}
}