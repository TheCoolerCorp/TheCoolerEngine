#include "../../../Include/Components/CustomComponents/PressurePlateComponent.h"

#include "UiHelper.h"
#include "Debugging/TCLogger.h"
#include "GamePlay/Components/ReflectionComponents/ComponentRegistry.h"
#include "GamePlay/Others/Scene.h"
static inline bool pressurePlateRegistered = Engine::GamePlay::AutoRegisterComponent<Editor::GamePlay::PressurePlateComponent>::registered;

namespace Editor::GamePlay
{
	nlohmann::ordered_json PressurePlateComponent::Serialize()
	{
		nlohmann::ordered_json t_json;
		t_json["transformComponentId"] = m_transformComponentId;
		t_json["collisionListenerGameObjectId"] = m_collisionListenerGameObjectId;
		t_json["doorGameObjectId"] = m_doorGameObjectId;

		t_json["position"] = 
		{
					{"x", m_doorActivePos.x},
					{"y", m_doorActivePos.y},
					{"z", m_doorActivePos.z}
		};

		return t_json;
	}

	void PressurePlateComponent::Deserialize(const nlohmann::ordered_json& a_json)
	{
		m_transformComponentId = a_json["transformComponentId"];
		m_collisionListenerGameObjectId = a_json["collisionListenerGameObjectId"];
		m_doorGameObjectId = a_json["doorGameObjectId"];

		const auto& t_pos = a_json.at("position");
		m_doorActivePos = {
			t_pos.at("x").get<float>(),
			t_pos.at("y").get<float>(),
			t_pos.at("z").get<float>()
		};
	}

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
		ImGui::Text("Pressed: %s", m_pressed ? "true" : "false");
		ImGui::Text("Collision Target");
		ImGui::SetItemTooltip("Set the target that will trigger the pressure plate component. Needs a ");

		std::string t_text;
		if (m_collisionListenerGameObjectId == -1)
		{
			t_text = "Not Set!";
		}
		else
		{

			t_text = m_scene->GetGameObject(m_collisionListenerGameObjectId)->GetName();
		}
		UiHelper::UiAddObjectDragDropTarget(t_text.c_str(), m_uid, m_collisionListenerGameObjectId);

		if (ImGui::TreeNode("Door Transform"))
		{
			//door transform that will be manipulated when the pressure plate is pressed
			ImGui::Text("Door Transform");
			ImGui::SetItemTooltip("The target that will be manipulated when the pressure plate is pressed.");
			if (m_collisionListenerGameObjectId == -1)
			{
				t_text = "Not Set!";
			}
			else
			{
				t_text = m_scene->GetGameObject(m_doorGameObjectId)->GetName();
			}
			UiHelper::UiAddObjectDragDropTarget(t_text.c_str(), m_uid, m_doorGameObjectId);

			UiHelper::UiDisplayVec3("Move Target Pos", m_uid, m_doorActivePos);
			ImGui::SetItemTooltip("The position the door will be in when the pressure plate is pressed.");

			ImGui::TreePop();
		}
		}

	void PressurePlateComponent::Start()
	{
		EditorGameComponent::Start();
		m_transformComponentId = GetGameObject()->GetComponentID<Engine::GamePlay::TransformComponent>();
		if (Engine::GamePlay::RigidBodyComponent* t_component = GetGameObject()->GetComponent<Engine::GamePlay::RigidBodyComponent>())
		{
			t_component->GetOnTriggerEnterEvent().AddListener([this](Engine::GamePlay::RigidBodyComponent* a_component) {this->OnColisionEnter(a_component);});
			t_component->GetOnTriggerExitEvent().AddListener([this](Engine::GamePlay::RigidBodyComponent* a_component) {this->OnColisionExit(a_component);});
		}
		else
		{
			//Log error on the editor logger
			TCLOG_WARNING("Object Has no RigidBodyComponent!");
		}
	}

	void PressurePlateComponent::Update(float a_deltatime)
	{
		EditorGameComponent::Update(a_deltatime);
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
			if (component->GetGameObjectID() == m_collisionListenerGameObjectId)
			{
				m_pressed = true;
				m_pressurePlatePressedEvent.Invoke(true);
			}
		}
	}

	void PressurePlateComponent::OnColisionExit(Engine::GamePlay::RigidBodyComponent* component)
	{
		if (component)
		{
			if (component->GetGameObjectID() == m_collisionListenerGameObjectId)
			{
				m_pressed = false;
				m_pressurePlatePressedEvent.Invoke(false);
			}
		}
	}
}
