#include "../Inlude/InspectorComponent/UiTransformComponent.h"

#include "imgui.h"
#include "Math/vec3.h"

using namespace Editor::EditorLayer::Ui;
using namespace Engine::Math;

Editor::EditorLayer::Ui::UiTransformComponent::~UiTransformComponent()
{
}

void Editor::EditorLayer::Ui::UiTransformComponent::Create()
{
}

void Editor::EditorLayer::Ui::UiTransformComponent::UiDraw()
{
	Transform* t_transform = m_transform->GetTransform();
	vec3 t_pos = t_transform->GetPosition();
	vec3 t_rot = quat::ToEulerAngles(t_transform->GetRotation());
	vec3 t_scale = t_transform->GetScale();

	float t_fPos[4]  = { t_pos.x, t_pos.y, t_pos.z, 0.f };
	float t_fRot[4] = { t_rot.x, t_rot.y, t_rot.z, 0.f };
	float t_fScale[4] = { t_scale.x, t_scale.y, t_scale.z, 0.f };

	std::string t_uid = std::to_string(m_uid) + std::to_string(m_transform->GetID());

	ImGui::SeparatorText(("Transform##"+ t_uid).c_str());
	if(ImGui::DragFloat3(("Position##" + t_uid).c_str(), t_fPos, 0.1f,-FLT_MAX, +FLT_MAX))
	{
		t_transform->SetPosition(vec3(t_fPos[0], t_fPos[1], t_fPos[2]));
	}
	if(ImGui::DragFloat3(("Rotation##" + t_uid).c_str(), t_fRot, 0.1f, -FLT_MAX, +FLT_MAX))
	{
		t_transform->SetRotation(quat(vec3(t_fRot[0], t_fRot[1], t_fRot[2])));
	}
	if(ImGui::DragFloat3(("Scale##" + t_uid).c_str(), t_fScale, 0.1f, -FLT_MAX, +FLT_MAX))
	{
		t_transform->SetScale(vec3(t_fScale[0], t_fScale[1], t_fScale[2]));
	}
}

void Editor::EditorLayer::Ui::UiTransformComponent::Destroy()
{
}



