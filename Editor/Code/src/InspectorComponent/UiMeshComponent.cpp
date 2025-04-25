#include "../Include/InspectorComponent/UiMeshComponent.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"

Editor::EditorLayer::Ui::UiMeshComponent::~UiMeshComponent()
{
}

void Editor::EditorLayer::Ui::UiMeshComponent::Create()
{
	//create image descriptor set
	m_dSet = ImGui_ImplVulkan_AddTexture(m_meshComp->GetTexture()->GetImage()->CastVulkan()->GetSampler(), m_meshComp->GetTexture()->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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
		ImGui::Text(("Path: " + m_meshComp->GetTexture()->GetPath()).c_str());
		ImGui::Text(("Size: " + std::to_string(m_meshComp->GetTexture()->GetWidth())+"x"+ std::to_string(m_meshComp->GetTexture()->GetHeight())).c_str());
		float t_displaySize = ImGui::GetWindowWidth() - 50.f;
		if (t_displaySize < 10.f)
			t_displaySize = 10.f;
		if (t_displaySize > 200.f)
			t_displaySize = 200.f;
		ImGui::Image(reinterpret_cast<ImTextureID>(m_dSet), ImVec2{ t_displaySize, t_displaySize });
		ImGui::SetItemTooltip((std::to_string(m_meshComp->GetTexture()->GetWidth()) + "x" + std::to_string(m_meshComp->GetTexture()->GetHeight())).c_str());
		ImGui::TreePop();
	}
	
}

void Editor::EditorLayer::Ui::UiMeshComponent::Destroy()
{
	ImGui_ImplVulkan_RemoveTexture(m_dSet);
	m_dSet = nullptr;
}
