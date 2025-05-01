#include "../Include/InspectorComponent/UiMeshComponent.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "Core/Interfaces/IObjectDescriptor.h"
#include "GamePlay/Others/Scene.h"
#include "GamePlay/Systems/RenderSystem.h"

Editor::EditorLayer::Ui::UiMeshComponent::~UiMeshComponent()
{
}

void Editor::EditorLayer::Ui::UiMeshComponent::Create()
{
	//create image descriptor set
	m_dSet = ImGui_ImplVulkan_AddTexture(m_meshComp->GetMaterial()->GetAlbedo()->GetImage()->CastVulkan()->GetSampler(), m_meshComp->GetMaterial()->GetAlbedo()->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiDraw()
{
	
	ImGui::SeparatorText("Mesh Component");
	ImGuiTreeNodeFlags t_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx(("Mesh Info: ##"+std::to_string(m_uid)).c_str(), t_flags))
	{
		ImGui::Text(("Path: "+m_meshComp->GetMesh()->GetPath()).c_str());
		ImGui::Text(("Vertices: "+std::to_string(m_meshComp->GetMesh()->GetNbVertices())).c_str());
		ImGui::Text(("Indices: "+std::to_string(m_meshComp->GetMesh()->GetNbIndices())).c_str());
		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx(("Texture Info: ##" + std::to_string(m_uid)).c_str(), t_flags))
	{
		ImGui::Text(("Path: " + m_meshComp->GetMaterial()->GetAlbedo()->GetPath()).c_str());
		ImGui::Text(("Size: " + std::to_string(m_meshComp->GetMaterial()->GetAlbedo()->GetWidth())+"x"+ std::to_string(m_meshComp->GetMaterial()->GetAlbedo()->GetHeight())).c_str());
		float t_displaySize = ImGui::GetWindowWidth() - 50.f;
		if (t_displaySize < 10.f)
			t_displaySize = 10.f;
		if (t_displaySize > 200.f)
			t_displaySize = 200.f;
		ImGui::Image(reinterpret_cast<ImTextureID>(m_dSet), ImVec2{ t_displaySize, t_displaySize });
		AddDragDropImageTarget();
		ImGui::SetItemTooltip((std::to_string(m_meshComp->GetMaterial()->GetAlbedo()->GetWidth()) + "x" + std::to_string(m_meshComp->GetMaterial()->GetAlbedo()->GetHeight())).c_str());
		ImGui::TreePop();
	}
	
}

void Editor::EditorLayer::Ui::UiMeshComponent::Destroy()
{
	if (m_dSet != VK_NULL_HANDLE)
	{
		vkDeviceWaitIdle(m_layer->GetRenderer()->GetLogicalDevice()->CastVulkan()->GetVkDevice());
		ImGui_ImplVulkan_RemoveTexture(m_dSet);
		m_dSet = VK_NULL_HANDLE;
	}
}

void Editor::EditorLayer::Ui::UiMeshComponent::AddDragDropImageTarget()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* t_payload = ImGui::AcceptDragDropPayload("IMAGE_PATH_PAYLOAD"))
		{
			const char* t_path = static_cast<const char*>(t_payload->Data);
			m_meshComp->GetMaterial()->SetAlbedo(t_path, m_layer->GetRenderer());
			int t_id = m_window->GetSelectedObject()->GetId();
			vkDeviceWaitIdle(m_layer->GetRenderer()->GetLogicalDevice()->CastVulkan()->GetVkDevice());
			m_layer->GetScene()->GetRenderSystem()->GetMeshDescriptor(t_id)->SetTexture(
				m_layer->GetRenderer()->GetLogicalDevice(), m_meshComp->GetMaterial()->GetAlbedo()->GetImage(), 1, 1);
		}
		ImGui::EndDragDropTarget();
	}
}
