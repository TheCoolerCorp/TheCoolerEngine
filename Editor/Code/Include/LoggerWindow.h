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
		void Destroy() override;

	private:
		int m_maxLogs = 10000;

		std::vector<Debugging::LogInfo> m_logs;
		Debugging::TCLogger* m_logger = nullptr;
	};
}

#endif // !UILOGGERWINDOW_H
