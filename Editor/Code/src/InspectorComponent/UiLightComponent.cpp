#include "../Include/InspectorComponent/UiLightComponent.h"
#include "GamePlay/Others/GameObject.h"

Editor::EditorLayer::Ui::UiLightComponent::~UiLightComponent()
{
}

void Editor::EditorLayer::Ui::UiLightComponent::Create()
{
}

void Editor::EditorLayer::Ui::UiLightComponent::UiDraw()
{
	ImGui::SeparatorText("Light Component");
	ImGui::SameLine();
	if (ImGui::Button(("Remove##Light" + std::to_string(m_uid)).c_str()))
	{
		m_window->GetSelectedObject()->RemoveComponent<Engine::GamePlay::LightComponent>();
		m_window->MarkOutOfDate();
		return;
	}

	Engine::Math::vec3 t_color = m_lightComp->GetLight().GetColor();

	float t_colorf[4] = { t_color.x, t_color.y, t_color.z, 0 };
	float t_intensity = m_lightComp->GetLight().GetIntensisty();

	if (ImGui::ColorEdit3(("Light Color##" + std::to_string(m_uid)).c_str(), t_colorf))
	{
		m_lightComp->GetLight().SetColor(Engine::Math::vec3(t_colorf[0], t_colorf[1], t_colorf[2]));
	}
	if (ImGui::DragFloat(("Intensity##" + std::to_string(m_uid)).c_str(), &t_intensity, 0.1f, 0, FLT_MAX))
	{
		m_lightComp->GetLight().SetIntensity(t_intensity);
	}
}

void Editor::EditorLayer::Ui::UiLightComponent::Destroy()
{
}
