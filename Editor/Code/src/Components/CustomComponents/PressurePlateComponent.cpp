#include "../../../Include/Components/CustomComponents/PressurePlateComponent.h"
#include "../../../Include/Components/ComponentRegistry.h"
#include "GamePlay/Others/Scene.h"
static inline bool pressurePlateRegistered = Editor::GamePlay::AutoRegisterComponent<Editor::GamePlay::PressurePlateComponent>::registered;

namespace Editor::GamePlay
{
	

	void PressurePlateComponent::DrawUI()
	{
		ImGui::SeparatorText("Pressure Plate Component");
		ImGui::SameLine();
		if (ImGui::Button(("Remove##PressurePlate" + std::to_string(m_uid)).c_str()))
		{
			m_uiWindow->MarkOutOfDate();
			GetGameObject()->RemoveComponent<PressurePlateComponent>();
			return;
		}
		ImGui::Text("Collision Target");
		ImGui::SetItemTooltip("Set the target that will trigger the pressure plate component. Needs a ");

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);


		// Compute child size manually (text size + padding * 2)
		std::string t_text;
		if (m_collisionListenerGameObject == -1)
		{
			t_text = "Not Set!";
		}
		else
		{
			
			t_text = m_scene->GetGameObject(m_collisionListenerGameObject)->GetName();
		}
		ImVec2 t_textSize = ImGui::CalcTextSize(t_text.c_str());
		ImVec2 t_childSize = ImVec2(t_textSize.x + 8, t_textSize.y + 8);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
		ImGui::BeginChild(("RotateOverrideDragDropTarget" + std::to_string(m_uid)).c_str(), t_childSize, true, ImGuiChildFlags_Border);
		ImGui::PopStyleVar(2);
		ImGui::Text(t_text.c_str());
		ImGui::EndChild();
	}

	void PressurePlateComponent::Start()
	{
		EditorGameComponent::Start();
		m_transformComponentId = GetGameObject()->GetComponentID<Engine::GamePlay::TransformComponent>();
	}

	void PressurePlateComponent::Update()
	{
		EditorGameComponent::Update();
	}

	void PressurePlateComponent::SceneUpdate()
	{
		EditorGameComponent::SceneUpdate();
	}

	void PressurePlateComponent::ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
	{
		EditorGameComponent::ProcessInputs(a_inputHandler, a_deltaTime);
	}

	void PressurePlateComponent::RemoveComponent(int a_id)
	{
		Engine::GamePlay::ServiceLocator::GetGameComponentSystem()->RemoveComponent(a_id);
	}

	PressurePlateComponent* PressurePlateComponent::GetComponent(int a_id)
	{
		return static_cast<PressurePlateComponent*>(Engine::GamePlay::ServiceLocator::GetGameComponentSystem()->GetComponent(a_id));
	}

	void PressurePlateComponent::OnColisionEnter(Engine::GamePlay::RigidBodyComponent* component)
	{
		if (component)
		{
			if (component->GetGameObjectID() == m_collisionListenerGameObject)
			{
				m_pressurePlatePressedEvent.Invoke(true);
			}
		}
	}

	void PressurePlateComponent::OnColisionExit(Engine::GamePlay::RigidBodyComponent* component)
	{
		if (component)
		{
			if (component->GetGameObjectID() == m_collisionListenerGameObject)
			{
				m_pressurePlatePressedEvent.Invoke(false);
			}
		}
	}

	void PressurePlateComponent::UiAddCollisionTargetDragDropTarget()
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_OBJECT_PAYLOAD"))
			{
				int t_id = *static_cast<int*>(payload->Data);
				if (t_id != -1)
				{
					m_collisionListenerGameObject = t_id;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}
