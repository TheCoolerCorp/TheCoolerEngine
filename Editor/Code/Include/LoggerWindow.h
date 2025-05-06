#ifndef UILOGGERWINDOW_H
#define UILOGGERWINDOW_H

#include "UiWindow.h"
#include "Debugging/TCLogger.h"

namespace Editor::EditorLayer::Ui
{
	class UiLoggerWindow : public UiWindow
	{
	public:
		UiLoggerWindow(Engine::Core::Renderer* a_renderer, ImGuiLayer* a_layer)
			: UiWindow(a_renderer, a_layer)
		{
			m_name = "Logger";
			m_logger = &Debugging::TCLogger::Get();
		}
		~UiLoggerWindow() override;
		void Create() override;
		void UiDraw() override;
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;
		void Destroy() override;
		void NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object) override;

	private:
		Debugging::TCLogger* m_logger = nullptr;
		Debugging::LogInfo m_selectedLog;

		int GetLogCount() { return static_cast<int>(m_logger->GetPendingLogs().size()); }
		bool IsValidLog();
		std::string GetTimeString(const std::tm& a_time)
		{
			std::ostringstream oss;
			oss << std::put_time(&a_time, "%H:%M:%S");
			return oss.str();
		}
	};
}

#endif 
