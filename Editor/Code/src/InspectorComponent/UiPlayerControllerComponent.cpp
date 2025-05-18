#include "../../Include/InspectorComponent/UiPlayerControllerComponent.h"

#include "Gameplay/ServiceLocator.h"
#include "GamePlay/Others/Scene.h"

namespace Editor::EditorLayer::Ui
{

	UiPlayerControllerComponent::~UiPlayerControllerComponent()
	{
	}

	void UiPlayerControllerComponent::Create()
	{
		
	}

	void UiPlayerControllerComponent::Destroy()
	{
		
	}

	void UiPlayerControllerComponent::AddDragDropRotateTransformOverride()
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_OBJECT_PAYLOAD"))
			{
				int t_id = *static_cast<int*>(payload->Data);
				if (t_id != -1)
				{
					int t_transformId = m_layer->GetScene()->GetGameObject(t_id)->GetComponentID<Engine::GamePlay::TransformComponent>();
					m_playerController->OverrideRotateTransform(t_transformId);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	void UiPlayerControllerComponent::UiDraw()
	{
		ImGui::SeparatorText("PlayerController Component");

		float t_maxSpeed = m_playerController->GetMaxSpeed();
		float t_rotationSpeed = m_playerController->GetRotationSpeed();
		float t_jumpForce = m_playerController->GetJumpForce();
		float t_moveSpeed = m_playerController->GetMoveSpeed();
		ImGui::SameLine();
		if (ImGui::Button(("Remove##PlayerController" + std::to_string(m_uid)).c_str()))
		{
			m_window->GetSelectedObject()->RemoveComponent<Engine::GamePlay::PlayerControllerComponent>();
			m_window->MarkOutOfDate();
			return;
		}

		if (ImGui::DragFloat(("Move Speed##"+std::to_string(m_uid)).c_str(), &t_moveSpeed, 0.1f, 0, FLT_MAX))
		{
			m_playerController->SetMoveSpeed(t_moveSpeed);
		}
		if (ImGui::DragFloat(("Max Speed##" + std::to_string(m_uid)).c_str(), &t_maxSpeed, 0.1f, 0, FLT_MAX))
		{
			m_playerController->SetMaxSpeed(t_maxSpeed);
		}
		if (ImGui::DragFloat(("Sensivity##" + std::to_string(m_uid)).c_str(), &t_rotationSpeed, 0.1f, 0, FLT_MAX))
		{
			m_playerController->SetRotationSpeed(t_rotationSpeed);
		}
		if (ImGui::DragFloat(("Jump Force##" + std::to_string(m_uid)).c_str(), &t_jumpForce, 0.1f, 0, FLT_MAX))
		{
			m_playerController->SetJumpForce(t_jumpForce);
		}

		ImGui::Text("Transform Rotate Override:");
		ImGui::SetItemTooltip("Allows you to override the object that is being rotated when you move the mouse. Accepts drag & drop");
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);

		// Compute child size manually (text size + padding * 2)
		std::string t_text;
		if (m_playerController->GetRotateTransformId() == -1)
		{
			t_text = "Not Overwritten";
		}
		else
		{
			Engine::GamePlay::TransformComponent* t_component = Engine::GamePlay::ServiceLocator::GetTransformSystem()->GetComponent(m_playerController->GetRotateTransformId());
			t_text = m_layer->GetScene()->GetGameObject(t_component->GetGameObjectID())->GetName() + "Transform";
		}
		ImVec2 t_textSize = ImGui::CalcTextSize(t_text.c_str());
		ImVec2 t_childSize = ImVec2(t_textSize.x + 8, t_textSize.y + 8);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
		ImGui::BeginChild(("RotateOverrideDragDropTarget" + std::to_string(m_uid)).c_str(), t_childSize, true, ImGuiChildFlags_Border);
		ImGui::PopStyleVar(2);
		ImGui::Text(t_text.c_str());
		ImGui::EndChild();
		AddDragDropRotateTransformOverride();
	}


}
