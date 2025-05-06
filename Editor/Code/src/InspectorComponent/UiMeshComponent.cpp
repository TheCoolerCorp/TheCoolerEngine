#include <algorithm>

#include "../Include/InspectorComponent/UiMeshComponent.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "Core/Audio/SoundManager.h"
#include "Core/Interfaces/IObjectDescriptor.h"
#include "GamePlay/Others/Scene.h"
#include "GamePlay/Systems/RenderSystem.h"
#include "GamePlay/Others/GameObject.h"

Editor::EditorLayer::Ui::UiMeshComponent::~UiMeshComponent()
{
}

void Editor::EditorLayer::Ui::UiMeshComponent::Create()
{
	m_material = m_meshComp->GetMaterial();
	//create image descriptor set
	m_isOutOfDate = true;
	CreateImageDescriptorSets();
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiDraw()
{
	if (m_isOutOfDate)
	{
		RefreshImageDescriptorSets();
		Engine::Core::Audio::SoundManager::Get().PlaySound("Assets/Sounds/sparkle.wav");
	}
	ImGui::SeparatorText("Mesh Component");
	ImGui::SameLine();
	if (ImGui::Button(("Remove##Mesh"+std::to_string(m_uid)).c_str()))
	{
		m_window->GetSelectedObject()->RemoveComponent<Engine::GamePlay::MeshComponent>();
		m_window->MarkOutOfDate();
		return;
	}
	ImGuiTreeNodeFlags t_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx(("Mesh Info: ##"+std::to_string(m_uid)).c_str(), t_flags))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);

		// Compute child size manually (text size + padding * 2)
		std::string t_text = "Path: " + m_meshComp->GetMesh()->GetPath();
		ImVec2 t_textSize = ImGui::CalcTextSize(t_text.c_str());
		ImVec2 t_childSize = ImVec2(t_textSize.x + 8, t_textSize.y + 8);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
		ImGui::BeginChild(("MeshDragDropTarget" + std::to_string(m_uid)).c_str(), t_childSize, true, ImGuiChildFlags_Border);
		ImGui::PopStyleVar(2);
		ImGui::Text(t_text.c_str());
		ImGui::EndChild();
		UiAddDragDropMeshTarget();
		
		ImGui::Text(("Vertices: "+std::to_string(m_meshComp->GetMesh()->GetNbVertices())).c_str());
		ImGui::Text(("Indices: "+std::to_string(m_meshComp->GetMesh()->GetNbIndices())).c_str());
		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx(("Texture Info: ##" + std::to_string(m_uid)).c_str(), t_flags))
	{
		//ImGui::Combo for the material type
		int t_itemSelectedIdx = 0;
		if (m_material->GetType() == Engine::GamePlay::LIT)
			t_itemSelectedIdx = 1;

		const char* t_items[] = { "Unlit", "Lit" };

		if (ImGui::BeginCombo(("Material Type##"+std::to_string(m_uid)).c_str(), t_items[t_itemSelectedIdx]))
		{
			for (int n = 0; n < IM_ARRAYSIZE(t_items); n++)
			{
				const bool t_isSelected = (t_itemSelectedIdx == n);
				if (ImGui::Selectable(t_items[n], t_isSelected))
				{
					t_itemSelectedIdx = n;
					if (t_itemSelectedIdx == 0)
						m_material->SetType(Engine::GamePlay::UNLIT);
					else
						m_material->SetType(Engine::GamePlay::LIT);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (t_isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SeparatorText("Albedo");
		UiDrawImageInfo(ALBEDO);
		if (m_material->GetType() == Engine::GamePlay::LIT)
		{
			ImGui::SeparatorText("Normal");
			UiDrawImageInfo(NORMAL);
			ImGui::SeparatorText("Metallic");
			UiDrawImageInfo(METALLIC);
			ImGui::SeparatorText("Roughness");
			UiDrawImageInfo(ROUGHNESS);
			ImGui::SeparatorText("Ambient Occlusion");
			UiDrawImageInfo(AMBIENTOCCLUSION);
		}
		ImGui::TreePop();
	}
	
}

void Editor::EditorLayer::Ui::UiMeshComponent::Destroy()
{
	ClearImageDescriptorSets();
}

void Editor::EditorLayer::Ui::UiMeshComponent::CreateImageDescriptorSets()
{
	

	const int t_id = m_window->GetSelectedObject()->GetComponentID<Engine::GamePlay::MeshComponent>();
	Engine::Core::RHI::IObjectDescriptor* t_descriptor = m_layer->GetScene()->GetRenderSystem()->GetMeshDescriptor(t_id);

	m_imageSets[ALBEDO] = VK_NULL_HANDLE;
	m_imageSets[NORMAL] = VK_NULL_HANDLE;
	m_imageSets[METALLIC] = VK_NULL_HANDLE;
	m_imageSets[ROUGHNESS] = VK_NULL_HANDLE;
	m_imageSets[AMBIENTOCCLUSION] = VK_NULL_HANDLE;
	if (!t_descriptor)
		return;

	m_isOutOfDate = false;

	if (m_material->HasAlbedo())
	{
		Engine::Ref<Engine::Resource::Texture> t_albedo = m_material->GetAlbedo();

		if (t_albedo->IsCreated())
		{
			m_imageSets[ALBEDO] = ImGui_ImplVulkan_AddTexture(t_albedo->GetImage()->CastVulkan()->GetSampler(), m_meshComp->GetMaterial()->GetAlbedo()->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			t_descriptor->SetTexture(m_layer->GetRenderer()->GetLogicalDevice(), t_albedo->GetImage(), 1, 1);
		}
		else
		{
			t_albedo->CreateImage(m_layer->GetRenderer());
			m_isOutOfDate = true;
		}
	}
	if (m_material->HasNormal())
	{
		Engine::Ref<Engine::Resource::Texture> t_normal = m_material->GetNormal();

		if (t_normal->IsCreated())
		{
			m_imageSets[NORMAL] = ImGui_ImplVulkan_AddTexture(t_normal->GetImage()->CastVulkan()->GetSampler(), m_meshComp->GetMaterial()->GetNormal()->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			t_descriptor->SetTexture(m_layer->GetRenderer()->GetLogicalDevice(), t_normal->GetImage(), 2, 1);
		}
		else
		{
			t_normal->CreateImage(m_layer->GetRenderer());
			m_isOutOfDate = true;
		}
	}
	if (m_material->HasMetallic())
	{
		Engine::Ref<Engine::Resource::Texture> t_metallic = m_material->GetMetallic();

		if (t_metallic->IsCreated())
		{
			m_imageSets[METALLIC] = ImGui_ImplVulkan_AddTexture(t_metallic->GetImage()->CastVulkan()->GetSampler(), m_meshComp->GetMaterial()->GetMetallic()->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			t_descriptor->SetTexture(m_layer->GetRenderer()->GetLogicalDevice(), t_metallic->GetImage(), 3, 1);
		}
		else
		{
			t_metallic->CreateImage(m_layer->GetRenderer());
			m_isOutOfDate = true;
		}
	}
	if (m_material->HasRoughness())
	{
		Engine::Ref<Engine::Resource::Texture> t_roughness = m_material->GetRoughness();

		if (t_roughness->IsCreated())
		{
			m_imageSets[ROUGHNESS] = ImGui_ImplVulkan_AddTexture(t_roughness->GetImage()->CastVulkan()->GetSampler(), m_meshComp->GetMaterial()->GetRoughness()->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			t_descriptor->SetTexture(m_layer->GetRenderer()->GetLogicalDevice(), t_roughness->GetImage(), 4, 1);
		}
		else
		{
			t_roughness->CreateImage(m_layer->GetRenderer());
			m_isOutOfDate = true;
		}
	}
	if (m_material->HasAO())
	{
		Engine::Ref<Engine::Resource::Texture> t_ao = m_material->GetAO();

		if (t_ao->IsCreated())
		{
			m_imageSets[AMBIENTOCCLUSION] = ImGui_ImplVulkan_AddTexture(m_material->GetAO()->GetImage()->CastVulkan()->GetSampler(), m_meshComp->GetMaterial()->GetAO()->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			t_descriptor->SetTexture(m_layer->GetRenderer()->GetLogicalDevice(), t_ao->GetImage(), 5, 1);
		}
		else
		{
			t_ao->CreateImage(m_layer->GetRenderer());
			m_isOutOfDate = true;
		}
	}
	
}

void Editor::EditorLayer::Ui::UiMeshComponent::ClearImageDescriptorSets()
{
	m_layer->GetRenderer()->GetLogicalDevice()->WaitIdle();
	for (auto& t_set : m_imageSets)
	{
		if (t_set.second != VK_NULL_HANDLE)
		{
			ImGui_ImplVulkan_RemoveTexture(t_set.second);
			t_set.second = VK_NULL_HANDLE;
		}
	}
}

void Editor::EditorLayer::Ui::UiMeshComponent::RefreshImageDescriptorSets()
{
	ClearImageDescriptorSets();
	CreateImageDescriptorSets();
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiDrawImageInfo(ImageType a_type)
{
	if (HasImage(a_type))
	{
		ImGui::Text(("Path: " + GetPath(a_type)).c_str());
		ImGui::Text(("Size: " + GetSize(a_type)).c_str());
	}
	else
	{
		switch (a_type)
		{
		case ALBEDO:
			UiDrawAlbedoInfo();
			break;
		case METALLIC:
			UiDrawMetallicInfo();
			break;
		case ROUGHNESS:
			UiDrawRoughnessInfo();
			break;
		case AMBIENTOCCLUSION:
			UiDrawAoInfo();
			break;
		}
	}


	float t_displaySize = ImGui::GetWindowWidth() - 50.f;
	t_displaySize = std::max(t_displaySize, 10.f);
	t_displaySize = std::min(t_displaySize, 100.f);

	//We create a child containing the image with form of padding so that we have a visible container if no image can be drawn
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::BeginChild((ToString(a_type) + std::to_string(m_uid)).c_str(), ImVec2(t_displaySize, t_displaySize), ImGuiChildFlags_Border, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	if (HasImage(a_type))
		ImGui::Image(reinterpret_cast<ImTextureID>(m_imageSets[a_type]), ImVec2{t_displaySize, t_displaySize});
	ImGui::SetItemTooltip(GetSize(a_type).c_str());
	ImGui::EndChild();
	UiAddDragDropImageTarget(a_type);
	ImGui::PopStyleVar(1);
	if (!HasImage(a_type))
		return;
	if(ImGui::Button(("Clear " + ToString(a_type) + "##" + std::to_string(m_uid)).c_str()))
	{
		switch (a_type)
		{
		case ALBEDO:
			m_material->RemoveAlbedo();
			break;
		case NORMAL:
			m_material->RemoveNormal();
			break;
		case METALLIC:
			m_material->RemoveMetallic();
			break;
		case ROUGHNESS:
			m_material->RemoveRoughness();
			break;
		case AMBIENTOCCLUSION:
			m_material->RemoveAO();
			break;
		}
		m_material->SetNeedUpdate(true);
		m_isOutOfDate = true;
	}
	
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiAddDragDropImageTarget(ImageType a_type)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* t_payload = ImGui::AcceptDragDropPayload("IMAGE_PATH_PAYLOAD"))
		{
			const char* t_path = static_cast<const char*>(t_payload->Data);
			const int t_id = m_meshComp->GetUid();
			Engine::Core::RHI::IObjectDescriptor* t_descriptor = m_layer->GetScene()->GetRenderSystem()->GetMeshDescriptor(t_id);
			switch (a_type)
			{
			case ALBEDO:
				m_material->SetAlbedo(t_path, m_layer->GetRenderer());
				break;
			case NORMAL:
				m_material->SetNormal(t_path, m_layer->GetRenderer());
				break;
			case METALLIC:
				m_material->SetMetallic(t_path, m_layer->GetRenderer());
				break;
			case ROUGHNESS:
				m_material->SetRoughness(t_path, m_layer->GetRenderer());
				break;
			case AMBIENTOCCLUSION:
				m_material->SetAO(t_path, m_layer->GetRenderer());
				break;
			}
			m_material->SetNeedUpdate(true);
			m_isOutOfDate = true;
			m_material->SetNeedUpdate(true);
		}
		ImGui::EndDragDropTarget();
	}
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiAddDragDropMeshTarget()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* t_payload = ImGui::AcceptDragDropPayload("MESH_PATH_PAYLOAD"))
		{
			const char* t_path = static_cast<const char*>(t_payload->Data);
			m_meshComp->SetMesh(t_path, m_layer->GetRenderer());
			m_isOutOfDate = true;
		}
		ImGui::EndDragDropTarget();
	}
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiDrawAlbedoInfo()
{
	Engine::Math::vec3 t_albedo = m_material->GetMaterialValues().albedo;
	float t_colorf[4] = { t_albedo.x, t_albedo.y, t_albedo.z, 1.f };
	ImGui::Text("Color Value: ");
	if (ImGui::ColorEdit3(("Albedo Color##" + std::to_string(m_uid)).c_str(), t_colorf))
	{
		m_material->SetAlbedo(Engine::Math::vec3(t_colorf[0], t_colorf[1], t_colorf[2]));
	}
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiDrawMetallicInfo()
{
	float t_metallic = m_material->GetMaterialValues().metallic;
	ImGui::Text("Metallic Value: ");
	if (ImGui::DragFloat(("Metallic Value##" + std::to_string(m_uid)).c_str(), &t_metallic, 0.01f, 0.f, 1.f))
	{
		m_material->SetMetallic(t_metallic);
	}
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiDrawRoughnessInfo()
{
	float t_roughness = m_material->GetMaterialValues().roughness;
	ImGui::Text("Roughness Value: ");
	if (ImGui::DragFloat(("Roughness Value##" + std::to_string(m_uid)).c_str(), &t_roughness, 0.01f, 0.f, 1.f))
	{
		m_material->SetRoughness(t_roughness);
	}
}

void Editor::EditorLayer::Ui::UiMeshComponent::UiDrawAoInfo()
{
	float t_ao = m_material->GetMaterialValues().ao;
	ImGui::Text("AO Value: ");
	if (ImGui::DragFloat(("AO Value##" + std::to_string(m_uid)).c_str(), &t_ao, 0.01f, 0.f, 1.f))
	{
		m_material->SetAO(t_ao);
	}
}


bool Editor::EditorLayer::Ui::UiMeshComponent::HasImage(ImageType a_type)
{
	return m_imageSets[a_type] != VK_NULL_HANDLE;
}

/**
 * Helper function.
 * Returns the path of the specified image for the given type of the object we have currently selected
 */
std::string Editor::EditorLayer::Ui::UiMeshComponent::GetPath(ImageType a_type)
{
	switch (a_type)
	{
	case ALBEDO:
		if (!m_material->HasAlbedo())
			return "No Albedo";
		return m_material->GetAlbedo()->GetPath();
	case NORMAL:
		if (!m_material->HasNormal())
			return "No Normal";
		return m_material->GetNormal()->GetPath();
	case METALLIC:
		if (!m_material->HasMetallic())
			return "No Metallic";
		return m_material->GetMetallic()->GetPath();
	case ROUGHNESS:
		if (!m_material->HasRoughness())
			return "No Roughness";
		return m_material->GetRoughness()->GetPath();
	case AMBIENTOCCLUSION:
		if (!m_material->HasAO())
			return "No AO";
		return m_material->GetAO()->GetPath();
	}
	return "Unknown";
}

/**
 * Helper function.
 * Returns the size of the specified image for the given type of the object we have currently selected
 */
std::string Editor::EditorLayer::Ui::UiMeshComponent::GetSize(ImageType a_type)
{
	switch (a_type) {
	case ALBEDO:
		if (!m_material->HasAlbedo())
			return "";
		return std::to_string(m_material->GetAlbedo()->GetWidth()) + "x" + std::to_string(m_material->GetAlbedo()->GetHeight());
	case NORMAL:
		if (!m_material->HasNormal())
			return "";
		return std::to_string(m_material->GetNormal()->GetWidth()) + "x" + std::to_string(m_material->GetNormal()->GetHeight());
	case METALLIC:
		if (!m_material->HasMetallic())
			return "";
		return std::to_string(m_material->GetMetallic()->GetWidth()) + "x" + std::to_string(m_material->GetMetallic()->GetHeight());
	case ROUGHNESS:
		if (!m_material->HasRoughness())
			return "";
		return std::to_string(m_material->GetRoughness()->GetWidth()) + "x" + std::to_string(m_material->GetRoughness()->GetHeight());
	case AMBIENTOCCLUSION:
		if (!m_material->HasAO())
			return "";
		return std::to_string(m_material->GetAO()->GetWidth()) + "x" + std::to_string(m_material->GetAO()->GetHeight());
	}
	return "Unknown";
}

std::string Editor::EditorLayer::Ui::UiMeshComponent::ToString(ImageType a_type)
{
	switch (a_type)
	{
	case ALBEDO:
		return "Albedo";
	case NORMAL:
		return "Normal";
	case METALLIC:
		return "Metallic";
	case ROUGHNESS:
		return "Roughness";
	case AMBIENTOCCLUSION:
		return "Ambient Occlusion";
	default:
		return "Unknown";
	}
}