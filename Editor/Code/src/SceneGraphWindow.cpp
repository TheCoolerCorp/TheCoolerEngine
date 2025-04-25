#include "../Inlude/SceneGraphWindow.h"

#include "imgui.h"
#include "../Inlude/ImGuiLayer.h"
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
			DrawObject(m_scene->GetGameObject(i)->GetComponent<TransformComponent>()->GetID());
		}
	}
    if (m_scene->GetObjectCount() == 0)
		ImGui::Text("No objects in the scene :(. Go make some");
	ImGui::End();
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::DrawObject(int a_transformId)  
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
			if (ImGui::IsItemClicked())
			{
				m_layer->SetSelectedGameObject(t_object);
			}
   			return;
   		}

       bool t_open = ImGui::TreeNodeEx(std::to_string(t_object->GetId()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow, t_object->GetName().c_str());
       if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
       {
           m_layer->SetSelectedGameObject(t_object);
       }

       if (t_open)
       {
	       for (int t_id : t_object->GetChildrenTransformIDs())  
	       {  
	           DrawObject(t_id);  
	       }  
	       ImGui::TreePop();  
       }
       
       
   }  
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::Destroy()
{
	
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::CreateNameTextField(GameObject* a_object)
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
