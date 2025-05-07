#include "SceneGraphWindow.h"

#include "imgui.h"
#include "ImGuiLayer.h"
using namespace Engine::GamePlay;

Editor::EditorLayer::Ui::SceneGraphUiWindow::~SceneGraphUiWindow()
{
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::Create()
{
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiDraw()
{
    ImGui::Begin((m_name + "##" + std::to_string(m_uid)).c_str(), &m_open);
	for (int i = 0; i < m_scene->GetObjectCount(); i++)
	{
		if (m_scene->GetGameObject(i) != nullptr && !m_scene->GetGameObject(i)->HasParent())
		{
			UiDrawObject(m_scene->GetGameObject(i)->GetComponent<TransformComponent>()->GetID());
		}
	}
    if (m_scene->GetObjectCount() == 0)
		ImGui::Text("No objects in the scene :(. Go make some");
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.01f);
	ImGui::BeginChild(("Empty Child" + std::to_string(m_uid)).c_str(), ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
    ImGui::Dummy(ImGui::GetContentRegionAvail());
    UiAddRootDragDropTarget();
    ImGui::EndChild();
    if (ImGui::IsItemClicked())
	{
		m_layer->DeselectObject();
	}
	ImGui::PopStyleVar();
	if (ImGui::BeginPopupContextItem(("SceneGraphContextMenu##"+std::to_string(m_uid)).c_str()))
    {
		m_layer->UiDrawItemAddMenu();
		ImGui::EndPopup();
    }
	ImGui::End();
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
{
}

/**
 * Draws the scene graph recursively. Starts with the transform object specified by a_transformId
 * then draws all of its children. All as TreeNodes.
 * If the object has no children, it will be drawn as a leaf node.
 * @param a_transformId The transform id of the object to draw the TreeNode of
 */
void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiDrawObject(int a_transformId)  
{  
   TransformComponent* t_transform = m_transformSystem->GetComponent(a_transformId);  
   if (t_transform == nullptr)  
   {  
       return;  
   }  
   if (GameObject* t_object = m_scene->GetGameObject(t_transform->GetGameObjectID()))  
   {  
   		std::string t_nodeName = t_object->GetName() + " " + std::to_string(t_object->GetId());
   		if (!t_object->HasChildren())
   		{
	        ImGui::TreeNodeEx(std::to_string(t_object->GetId()).c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, t_object->GetName().c_str());
			UiAddDragDropSource(t_object);
			UiAddObjectDragDropTarget(t_object);
   			UiAddPopupContext(t_object);
   			if (ImGui::IsItemClicked())
			{
				m_layer->SetSelectedGameObject(t_object);
			}
   			return;
   		}

       bool t_open = ImGui::TreeNodeEx(std::to_string(t_object->GetId()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow, t_object->GetName().c_str());
	   UiAddDragDropSource(t_object);
	   UiAddObjectDragDropTarget(t_object);
   	   if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
       {
           m_layer->SetSelectedGameObject(t_object);
       }
       UiAddPopupContext(t_object);

       if (t_open)
       {
	       for (int t_id : t_object->GetChildrenTransformIDs())  
	       {  
	           UiDrawObject(t_id);  
	       }  
	       ImGui::TreePop();  
       }
       
       
   }  
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::Destroy()
{
	
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object)
{
}

/**
 * Creates an editable text field for the name of the object in the same line as the previous element.
 * This function is deprecated and no longer used.
 * @param a_object the object to create the name text field for
 */
void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiCreateNameTextField(GameObject* a_object)
{
    // Create a buffer for editing
    char buffer[128];
    strncpy_s(buffer, a_object->GetName().c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0)); // Transparent background
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);    // No border
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // No padding

    if (ImGui::InputText(("##" + std::to_string(a_object->GetId()) + "text").c_str(), buffer, sizeof(buffer)))
    {
        a_object->SetName(buffer);
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
}

/**
 * Adds a popup for the given object. This popup will be shown when the user right clicks on the object.
 * Its only functionality is to allow the user to delete the given element
 * @param a_object the concerned element for the popup
 */
void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiAddPopupContext(GameObject* a_object)
{
	if (ImGui::BeginPopupContextItem(("SceneGraphItemContextMenu##"+std::to_string(a_object->GetId())).c_str()))
	{
		if (ImGui::MenuItem(("Delete "+a_object->GetName()+"##"+std::to_string(m_uid)).c_str()))
		{
			m_layer->NotifyObjectRemoved(a_object);
			m_scene->RemoveGameObject(a_object->GetId());
		}
		ImGui::EndPopup();
	}
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiAddDragDropSource(Engine::GamePlay::GameObject* a_object)
{
	static int payloadId; //static because we do not want this value to become deprecated
	if (ImGui::BeginDragDropSource())
	{
		payloadId = a_object->GetComponentID<TransformComponent>();
		ImGui::SetDragDropPayload("SCENE_OBJECT_PAYLOAD", &payloadId, sizeof(payloadId));
		ImGui::Text(a_object->GetName().c_str());
		ImGui::EndDragDropSource();
	}
}

/**
 * Adds a drag and drop target to the given object.
 * This will allow the user to drag and drop objects onto this object.
 * To set them as children of this object.
 */
void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiAddObjectDragDropTarget(Engine::GamePlay::GameObject* a_object)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_OBJECT_PAYLOAD"))
		{
			int t_id = *static_cast<int*>(payload->Data);
			if (t_id != -1)
			{
				a_object->AddChild(t_id);
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiAddRootDragDropTarget()
{
	//finds the gameobject from the payload's transform id and clears its parent
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_OBJECT_PAYLOAD"))
		{
			int t_id = *static_cast<int*>(payload->Data);
			if (t_id != -1)
			{
				m_transformSystem->GetComponent(t_id)->RemoveParent();
			}
		}
		ImGui::EndDragDropTarget();
	}
}
