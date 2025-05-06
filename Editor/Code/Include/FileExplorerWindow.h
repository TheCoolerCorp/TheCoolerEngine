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
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;
		void Destroy() override;
		void NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object) override;

		static void SetRootPath(const std::filesystem::path& a_path) { m_rootPath = a_path; }
	private:
		bool m_previewImages = false;

		std::filesystem::path m_currentPath;
		static std::filesystem::path m_rootPath;

		static ImGuiTexture* m_folderTexture;
		static ImGuiTexture* m_imageTexture;
		static ImGuiTexture* m_fileTexture;

		std::unordered_map<std::string, ImGuiTexture*> m_textures;

		void SetCurrentPath(std::filesystem::path a_path);

		bool HasChildDirectories(const std::filesystem::path& a_path);
		bool IsImage(const std::filesystem::path& a_path);
		bool IsModel(const std::filesystem::path& a_path);

		std::filesystem::path TruncatePathToRoot(const std::filesystem::path& a_path);

		void LoadContextImages(const std::filesystem::path& a_path);
		void ClearContextImages();

		void UiDrawFileInfo();

		void UiDrawFileTree();
		void UiDrawFileTreeRecursive(const std::filesystem::path& a_path);

		void UiDrawTextCentered(std::string a_text);
		void UiDrawFileImage(const std::filesystem::directory_entry& a_path);
		void UiAddImageDragDropSource(const std::filesystem::path& a_path);
		void UiAddModelDragDropSource(const std::filesystem::path& a_path);
	};
}

#endif