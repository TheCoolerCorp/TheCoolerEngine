#include "../Include/UiHelper.h"

#include <string>

#include "imgui.h"

#include "imgui_internal.h"

/**
 * Generates a drag and drop target for a UI object. Returns the id of the found object, or -1 if none was found.
 * @param a_text The text that will be displayed in the drag drop target
 * @param a_id a unique identifier to give this drag & drop target. If one is reused, issues may arise.
 * @return 
 */
int UiHelper::UiAddObjectDragDropTarget(const char* a_text, int a_id)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);


	ImVec2 t_textSize = ImGui::CalcTextSize(a_text);
	ImVec2 t_childSize = ImVec2(t_textSize.x + 8, t_textSize.y + 8);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
	ImGui::BeginChild(a_text, t_childSize, true, ImGuiChildFlags_Border);
	ImGui::PopStyleVar(2);
	ImGui::Text(a_text);
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_OBJECT_PAYLOAD"))
		{
			int t_id = *static_cast<int*>(payload->Data);
			return t_id;
		}
		ImGui::EndDragDropTarget();
	}
	return -1;
}

void UiHelper::UiAddObjectDragDropTarget(const char* a_text, int a_id, int& a_outId)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);


	ImVec2 t_textSize = ImGui::CalcTextSize(a_text);
	ImVec2 t_childSize = ImVec2(t_textSize.x + 8, t_textSize.y + 8);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
	ImGui::BeginChild(a_text, t_childSize, true, ImGuiChildFlags_Border);
	ImGui::PopStyleVar(2);
	ImGui::Text(a_text);
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_OBJECT_PAYLOAD"))
		{
			a_outId = *static_cast<int*>(payload->Data);
		}
		ImGui::EndDragDropTarget();
	}
}

bool UiHelper::UiDisplayVec3(std::string a_label, int a_id, Engine::Math::vec3& a_vec3, float a_speed, float a_min,
                             float a_max)
{
	float t_val[4] = { a_vec3.x, a_vec3.y, a_vec3.z, 0.f };
	if (ImGui::DragFloat3((a_label + "##" + std::to_string(a_id)).c_str(), t_val, 0.1f, -FLT_MAX, +FLT_MAX))
	{
		a_vec3.x = t_val[0];
		a_vec3.y = t_val[1];
		a_vec3.z = t_val[2];
		return true;
	}
	return false;
}
