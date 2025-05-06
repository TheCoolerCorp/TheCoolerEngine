#include "InspectorWindow.h"

#include "imgui.h"
#include "../Include/ImGuiLayer.h"
#include "../Include/InspectorComponent/UiInspectorComponent.h"
#include "../Include/InspectorComponent/UiTransformComponent.h"
#include "../Include/InspectorComponent/UiMeshComponent.h"
#include "../Include/UiWindow.h"
#include "InspectorComponent/UiLightComponent.h"
#include "InspectorComponent/UiRigidbodyComponent.h"

Editor::EditorLayer::Ui::InspectorUiWindow::~InspectorUiWindow()
{
}

void Editor::EditorLayer::Ui::InspectorUiWindow::Create()
{
}

void Editor::EditorLayer::Ui::InspectorUiWindow::UiDraw()
{
	
	if (!m_locked)
	{
		RefreshSelectedObject();
	}
	if (m_isOutOfDate)
	{
		m_isOutOfDate = false;
		RefreshCurrentObject();
	}
	ImGui::Begin((m_name+"##"+std::to_string(m_uid)).c_str(), &m_open);
	if (m_selectedObject != nullptr)
	{
		ImGui::SeparatorText("Object Info");
		ImGui::Text(("Object ID: " + std::to_string(m_selectedObject->GetId())).c_str());
		ImGui::Text("Object Name: ");
		ImGui::SameLine();
		CreateNameTextField();
		ImGui::Text("Lock Selected Object");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
		ImGui::Checkbox("##Lock", &m_locked);
		for (InspectorComponent* t_objectComponent : m_objectComponents)
		{
			t_objectComponent->UiDraw();
		}
		ImGui::Separator();
		DrawComponentAddWindow();
	}
	else
	{
		ImVec2 windowSize = ImGui::GetWindowSize();
		const char* text = "No object selected!";
		ImVec2 textSize = ImGui::CalcTextSize(text);

		// Center position = (window size - text size) / 2
		ImVec2 pos = ImVec2(
			(windowSize.x - textSize.x) * 0.5f,
			(windowSize.y - textSize.y) * 0.5f
		);

		ImGui::SetCursorPos(pos);
		ImGui::Text("%s", text);
	}
	ImGui::End();
}

void Editor::EditorLayer::Ui::InspectorUiWindow::Destroy()
{
	ClearComponents();
	m_selectedObject = nullptr;
	m_open = false; 
}

void Editor::EditorLayer::Ui::InspectorUiWindow::ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
{
}

void Editor::EditorLayer::Ui::InspectorUiWindow::NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object)
{
	if (m_selectedObject)
	{
		if (m_selectedObject->GetId()==a_object->GetId())
			m_selectedObject = nullptr;
	}
}

bool Editor::EditorLayer::Ui::InspectorUiWindow::HasComponentOfType(UiComponentType a_type)
{
	for (InspectorComponent* t_component : m_objectComponents)
	{
		if (t_component->GetType() == a_type)
			return true;
	}
	return false;
}


/**
 * Checks if the currently selected GameObject is different to the one stored in the inspector.
 * If the inspector is not in a locked state, clear the previous components and rebuild all ui elements.
 */
void Editor::EditorLayer::Ui::InspectorUiWindow::RefreshSelectedObject()
{
	Engine::GamePlay::GameObject* t_object = m_layer->GetSelectedGameObject();
	if (!IsObjectOutOfDate())
		return;
	if (t_object == nullptr)
	{
		ClearComponents();
		m_selectedObject = nullptr;
		return;
	}
	
	m_selectedObject = t_object;
	RefreshCurrentObject();
}

void Editor::EditorLayer::Ui::InspectorUiWindow::RefreshCurrentObject()
{
	ClearComponents();
	for (Engine::GamePlay::ComponentType t_type : m_selectedObject->GetOwnedTypes())
	{
		switch (t_type)
		{
		case Engine::GamePlay::ComponentType::TRANSFORM:
			AddComponent(new UiTransformComponent(m_layer, m_selectedObject->GetComponent<Engine::GamePlay::TransformComponent>()));
			break;
		case Engine::GamePlay::ComponentType::MESH:
			AddComponent(new UiMeshComponent(m_layer, m_selectedObject->GetComponent<Engine::GamePlay::MeshComponent>()));
			break;
		case Engine::GamePlay::ComponentType::MATERIAL: //not implemented yet
			//AddComponent(new UiMaterialComponent(m_layer, m_selectedObject->GetComponent<MaterialComponent>()));
			break;
		case Engine::GamePlay::ComponentType::COLLIDERMESH:
			//AddComponent(new UiColliderMeshComponent(m_layer, m_selectedObject->GetComponent<ColliderMeshComponent>()));
			break;
		case Engine::GamePlay::ComponentType::RIGIDBODY:
			AddComponent(new UiRigidbodyComponent(m_layer, m_selectedObject->GetComponent<Engine::GamePlay::RigidBodyComponent>()));
			break;
		case Engine::GamePlay::ComponentType::LIGHT:
			AddComponent(new UiLightComponent(m_layer, m_selectedObject->GetComponent<Engine::GamePlay::LightComponent>()));
			break;
		}
	}
}

/**
 * Will return true if: 
 * - the current selected object is nullptr and the class's current object is not nullptr
 * - the current object for the class is nullptr and the selected object is not nullptr
 * - the class's current object's id is different from the selected object's id \n
 * always returns false is the window is "locked"
 */
bool Editor::EditorLayer::Ui::InspectorUiWindow::IsObjectOutOfDate() const
{
	if (m_locked)
		return false;
	const Engine::GamePlay::GameObject* t_object = m_layer->GetSelectedGameObject();
	if (m_selectedObject == nullptr && t_object == nullptr)
		return false;
	if (m_selectedObject == nullptr && t_object != nullptr)
		return true;
	if (m_selectedObject != nullptr && t_object == nullptr)
		return true;
	if (m_selectedObject->GetId() != t_object->GetId())
		return true;
	return false;
}

void Editor::EditorLayer::Ui::InspectorUiWindow::AddComponent(InspectorComponent* a_component)
{
	if (a_component == nullptr)
		return;
	m_objectComponents.push_back(a_component);
	a_component->SetUid(static_cast<int>(m_objectComponents.size())-1);
	a_component->SetInspectorWindow(this);
	a_component->Create();

}

void Editor::EditorLayer::Ui::InspectorUiWindow::ClearComponents()
{
	for (InspectorComponent* t_component : m_objectComponents)
	{
		t_component->Destroy();
		delete t_component;
	}
	m_objectComponents.clear();
}

void Editor::EditorLayer::Ui::InspectorUiWindow::CreateNameTextField()
{
	// Create a buffer for editing
	char buffer[128];
	strncpy_s(buffer, m_selectedObject->GetName().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0)); // Transparent background
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);    // No border
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // No padding

	if (ImGui::InputText(("##" + std::to_string(m_selectedObject->GetId()) + "text").c_str(), buffer, sizeof(buffer)))
	{
		m_selectedObject->SetName(buffer);
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
}

void Editor::EditorLayer::Ui::InspectorUiWindow::DrawComponentAddWindow()
{
	if (ImGui::BeginCombo(("##Add Component"+std::to_string(m_uid)).c_str(), "Add Component"))
	{
		//All objects have a transform by default, it is as such not needed here
		if (!HasComponentOfType(UiComponentType::MESH))
		{
			if (ImGui::Selectable("Mesh"))
			{
				m_selectedObject->AddComponent<Engine::GamePlay::MeshComponent>();
				//Workaround: if you dont set a mesh the program will crash on draw. So we set a default one
				m_selectedObject->GetComponent<Engine::GamePlay::MeshComponent>()->SetMesh("Assets/Meshes/empty.obj", m_renderer);
				m_selectedObject->GetComponent<Engine::GamePlay::MeshComponent>()->GetMaterial()->Create(Engine::GamePlay::UNLIT);
				RefreshCurrentObject();
			}
		}
		if (!HasComponentOfType(UiComponentType::RIGIDBODY))
		{
			if (ImGui::Selectable("Rigidbody"))
			{
				m_selectedObject->AddComponent<Engine::GamePlay::RigidBodyComponent>();
				RefreshCurrentObject();
			}
		}
		if (!HasComponentOfType(UiComponentType::LIGHT))
		{
			if (ImGui::Selectable("Light"))
			{
				m_selectedObject->AddComponent<Engine::GamePlay::LightComponent>();
				RefreshCurrentObject();
			}
		}
		ImGui::EndCombo();
	}
}
