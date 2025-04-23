#include "../Inlude/SceneGraphWindow.h"

#include "imgui.h"

using namespace Engine::GamePlay;

Editor::EditorLayer::Ui::SceneGraphUiWindow::~SceneGraphUiWindow()
{
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::Create()
{
}

void Editor::EditorLayer::Ui::SceneGraphUiWindow::UiDraw()
{
	ImGui::Begin(m_name.c_str());
	for (int i = 0; i < m_scene->GetObjectCount(); i++)
	{
		if (m_scene->GetGameObject(i) != nullptr && !m_scene->GetGameObject(i)->HasParent())
		{
			DrawObject(m_scene->GetGameObject(i)->GetComponent<TransformComponent>()->GetID());
		}
	}
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
       std::string nodeName = t_object->GetName() + " " + std::to_string(t_object->GetId());  
       if (ImGui::TreeNode(nodeName.c_str()))  
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
