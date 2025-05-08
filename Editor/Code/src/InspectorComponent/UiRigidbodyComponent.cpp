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
	/*ImGui::SameLine();
	if (ImGui::Button("Remove Component"))
	{
		m_window->GetSelectedObject()->RemoveComponent<Engine::GamePlay::RigidBodyComponent>();
		m_window->MarkOutOfDate();
		return;
	}*/

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

	
	UiShowColliderInfo();
	if (ImGui::DragFloat3(("Position##" + std::to_string(m_uid)).c_str(), t_fPos, 0.1f, -FLT_MAX, +FLT_MAX))
	{
		m_rigidBody->SetPosition(Engine::Math::vec3(t_fPos[0], t_fPos[1], t_fPos[2]));
	}
	if (ImGui::DragFloat3(("Rotation##" + std::to_string(m_uid)).c_str(), t_fRot, 0.1f, -FLT_MAX, +FLT_MAX))
	{
		m_rigidBody->SetRotation(Engine::Math::quat(Engine::Math::vec3(t_fRot[0], t_fRot[1], t_fRot[2])));
	}
	UiShowBodyType();
	UiShowCollisionLayer();
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::Destroy()
{
}

void Editor::EditorLayer::Ui::UiRigidbodyComponent::UiShowColliderInfo()
{
	ImGui::Text("Collider Type: ");
	ImGui::SameLine();
	switch (m_rigidBody->GetColliderType())
	{
	case Engine::Physics::ColliderType::BOX:
	{
		ImGui::Text("Box");
		Engine::Math::vec3 t_scale = m_rigidBody->GetBody()->GetScale();
		float t_fScale[4] = { t_scale.x, t_scale.y, t_scale.z, 0.f };

		if (ImGui::DragFloat3(("Scale##" + std::to_string(m_uid)).c_str(), t_fScale, 0.1f,0.01f, +FLT_MAX))
		{
			m_rigidBody->GetBody()->SetScale(Engine::Math::vec3(t_fScale[0], t_fScale[1], t_fScale[2]));
		}
		break;
	}
	case Engine::Physics::ColliderType::SPHERE:
	{
		ImGui::Text("Sphere");
		float t_radius = m_rigidBody->GetBody()->GetRadius();
		if (ImGui::DragFloat(("Radius##" + std::to_string(m_uid)).c_str(), &t_radius, 0.1f, -FLT_MAX, +FLT_MAX))
		{
			m_rigidBody->GetBody()->SetRadius(t_radius);
		}
		break;
	}
	case Engine::Physics::ColliderType::CAPSULE:
	{
		ImGui::Text("Capsule");
		float t_radius = m_rigidBody->GetBody()->GetRadius();
		float t_halfHeight = m_rigidBody->GetBody()->GetHalfHeight();
		if (ImGui::DragFloat(("HalfHeight##" + std::to_string(m_uid)).c_str(), &t_halfHeight, 0.1f, -FLT_MAX, +FLT_MAX))
		{
			m_rigidBody->GetBody()->SetHalfHeight(t_halfHeight);
		}
		if (ImGui::DragFloat(("Radius##" + std::to_string(m_uid)).c_str(), &t_radius, 0.1f, -FLT_MAX, +FLT_MAX))
		{
			m_rigidBody->GetBody()->SetRadius(t_radius);
		}
		break;
	}
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