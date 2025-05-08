#include "LoggerWindow.h"

#include "imgui.h"
#include "imgui_internal.h"

void Editor::EditorLayer::Ui::UiLoggerWindow::UiDraw()
{
	/*
	 * The first child draws all the logs using a ImGui clipper, allowing us to only draw the visible logs in the scrollable area,
	 * saving a ton of performance.
	 */
	ImGui::Begin((m_name + std::to_string(m_uid)).c_str(), &m_open);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::BeginChild("LogContainer", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y*0.6f), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);

	ImGuiListClipper t_clipper;
	t_clipper.Begin(GetLogCount(), 60.0f);
	std::vector<Debugging::LogInfo>& t_logs = m_logger->GetPendingLogs();
	while (t_clipper.Step())
	{
		for (int i = t_clipper.DisplayStart; i < t_clipper.DisplayEnd; i++)
		{
			Debugging::LogInfo& t_log = t_logs[i];
			switch (t_log.mLogLevel)
			{
			case Debugging::LogLevel::DEBUGLOG:
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 1.f, 0.f, 1.0f));
				break;
			case Debugging::LogLevel::VERBOSE:
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 1.f, 1.0f));
				break;
			case Debugging::LogLevel::INFO:
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 1.f, 0.f, 1.0f));
				break;
			case Debugging::LogLevel::WARNING:
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.5f, 0.f, 1.0f));
				break;
			case Debugging::LogLevel::ERROR:
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.f, 0.f, 1.0f));
				break;
			case Debugging::LogLevel::CRITICAL:
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.f, 0.f, 1.0f));
				break;
			}
			ImGui::BeginChild(("Log" + std::to_string(i)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 60), ImGuiChildFlags_Borders);
			ImGui::Text(GetTimeString(t_log.mLocalTime).c_str());
			ImGui::SameLine();
			ImGui::Text(t_log.mLocation);
			ImGui::SameLine();
			ImGui::Text(std::to_string(t_log.mLine).c_str());
			ImGui::Text(t_log.mMessage.c_str());
			ImGui::EndChild();
			if (ImGui::IsItemClicked())
			{
				m_selectedLog = t_log;
			}
			ImGui::PopStyleColor();
		}
	}
	/*
	 * The second child draws the selected log, if any is selected, as plain text.
	 */
	ImGui::EndChild();
	ImGui::BeginChild("SelectedInfo", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_HorizontalScrollbar);
	if (IsValidLog())
	{
		ImGui::Text(GetTimeString(m_selectedLog.mLocalTime).c_str());
		ImGui::TextWrapped(std::string(m_selectedLog.mLocation).c_str());
		ImGui::Text(std::to_string(m_selectedLog.mLine).c_str());
		ImGui::TextWrapped(m_selectedLog.mMessage.c_str());
	}
	ImGui::EndChild();
	ImGui::PopStyleVar(2);

	ImGui::End();

	
}

void Editor::EditorLayer::Ui::UiLoggerWindow::ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
{
}

Editor::EditorLayer::Ui::UiLoggerWindow::~UiLoggerWindow()
{
}

void Editor::EditorLayer::Ui::UiLoggerWindow::Create()
{
	TCLOG_VERBOSE("Wow");
	TCLOG_DEBUG("Hello world!");
	TCLOG_INFO("Bonjour Chris et Lucile!!!!!!!!! :3");
	TCLOG_WARNING("oopsies!!!!");
	TCLOG_ERROR("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed quis arcu bibendum, dictum purus id, condimentum diam. Donec molestie erat a lacus tincidunt commodo. Aliquam cursus iaculis eros ac tristique. Phasellus condimentum molestie justo. Cras quis lacus hendrerit, feugiat tellus in, rhoncus ante. Aenean vitae vulputate libero. Cras vulputate metus at libero dignissim, eu efficitur felis accumsan. Proin ac diam diam. Ut vehicula condimentum turpis ut ultrices. Fusce finibus dui lectus, sed congue mi tempus eu. Donec ac auctor odio. Aenean risus dui, posuere nec suscipit nec, mollis eu quam. Cras mattis nulla et ex egestas egestas. Donec facilisis leo nec mi ultrices tincidunt. Quisque vitae rhoncus nisi.");
	TCLOG_CRITICAL("Wow you done messed up");
}

void Editor::EditorLayer::Ui::UiLoggerWindow::Destroy()
{
}

void Editor::EditorLayer::Ui::UiLoggerWindow::NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object)
{
}

//checks if m_selectedLog is valid
bool Editor::EditorLayer::Ui::UiLoggerWindow::IsValidLog()
{
	if (m_selectedLog.mMessage.empty())
	{
		return false;
	}
	if (m_selectedLog.mLocation == nullptr)
	{
		return false;
	}
	if (m_selectedLog.mLine < 0)
	{
		return false;
	}
	return true;
}

