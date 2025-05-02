#ifndef UIFILEEXPLORERWINDOW_H
#define UIFILEEXPLORERWINDOW_H

#include "UIWindow.h"
#include "Resources/ImGuiTexture.h"

namespace Editor::EditorLayer::Ui
{
	class FileExplorerWindow : public UiWindow
	{
	public:
		FileExplorerWindow(Engine::Core::Renderer* a_renderer, ImGuiLayer* a_layer)
			: UiWindow(a_renderer, a_layer)
		{
			m_name = "Explorer";
		}
		~FileExplorerWindow() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

		static void SetRootPath(const std::filesystem::path& a_path) { m_rootPath = a_path; }
	private:

		std::filesystem::path m_currentPath;
		static std::filesystem::path m_rootPath;
		static ImGuiTexture* m_folderTexture;
		static ImGuiTexture* m_imageTexture;
		static ImGuiTexture* m_fileTexture;
		void DrawFileInfo();

		void DrawFileTree();
		void DrawFileTreeRecursive(const std::filesystem::path& a_path);

		bool HasChildDirectories(const std::filesystem::path& a_path);
		bool IsImage(const std::filesystem::path& a_path);

		std::filesystem::path TruncatePathToRoot(const std::filesystem::path& a_path);

		void DrawTextCentered(std::string a_text);
		void DrawFileImage(const std::filesystem::directory_entry& a_path);
		void AddImageDragDropSource(const std::filesystem::path& a_path);
	};
}

#endif