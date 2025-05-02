#include <algorithm>

#include "../Include/FileExplorerWindow.h"

#include "imgui.h"

std::filesystem::path Editor::EditorLayer::Ui::FileExplorerWindow::m_rootPath;
Editor::EditorLayer::Ui::ImGuiTexture* Editor::EditorLayer::Ui::FileExplorerWindow::m_folderTexture = nullptr;
Editor::EditorLayer::Ui::ImGuiTexture* Editor::EditorLayer::Ui::FileExplorerWindow::m_fileTexture = nullptr;
Editor::EditorLayer::Ui::ImGuiTexture* Editor::EditorLayer::Ui::FileExplorerWindow::m_imageTexture = nullptr;

Editor::EditorLayer::Ui::FileExplorerWindow::~FileExplorerWindow()
{
}

void Editor::EditorLayer::Ui::FileExplorerWindow::Create()
{
	if (!m_folderTexture)
	{
		m_folderTexture = new ImGuiTexture(m_renderer, "Assets/Textures/Ui/FileIcon.png");
	}
	if (!m_fileTexture)
	{
		m_fileTexture = new ImGuiTexture(m_renderer, "Assets/Textures/Ui/BlankFileIcon.png");
	}
	if (!m_imageTexture)
	{
		m_imageTexture = new ImGuiTexture(m_renderer, "Assets/Textures/Ui/ImageIcon.png");
	}
	m_rootPath = std::filesystem::current_path();
	for (const auto& entry : std::filesystem::directory_iterator(m_rootPath)) {
		if (entry.is_directory() && entry.path().filename() == "Assets") {
			m_rootPath = entry.path();
			break;
		}
	}
	m_currentPath = m_rootPath;
}

void Editor::EditorLayer::Ui::FileExplorerWindow::UiDraw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::Begin((m_name + "##" + std::to_string(m_uid)).c_str(), &m_open);

	float t_width = ImGui::GetContentRegionAvail().x*0.3f;
	t_width = std::min(t_width, 150.f);

	ImGui::BeginChild(("FileTree" + std::to_string(m_uid)).c_str(),ImVec2(t_width, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PopStyleVar(2);

	DrawFileTree();
	ImGui::EndChild();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SameLine();
	
	ImGui::BeginChild(("FileInfo" + std::to_string(m_uid)).c_str(), ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::BeginChild(("Toolbar" + std::to_string(m_uid)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 20.0f), ImGuiChildFlags_Borders);
	ImGui::PopStyleVar(2);
	if (ImGui::Button(("Back##" + std::to_string(m_uid)).c_str()))
	{
		if (m_currentPath.has_parent_path() && m_currentPath != m_rootPath)
		{
			m_currentPath = m_currentPath.parent_path();
		}
	}
	ImGui::SameLine();
	ImGui::Text(m_currentPath.string().c_str());
	ImGui::EndChild();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::BeginChild(("Browser" + std::to_string(m_uid)).c_str(), ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PopStyleVar(2);

	DrawFileInfo();
	ImGui::EndChild();

	ImGui::EndChild();

	ImGui::End();
}

void Editor::EditorLayer::Ui::FileExplorerWindow::Destroy()
{
}

/**
 * Draws the content of m_currentPath (the currently selected folder) in a ImGui table
 * With an image illustrating its type and its name. Folders can be double clicked to acess its child
 */
void Editor::EditorLayer::Ui::FileExplorerWindow::DrawFileInfo()
{
	int t_filesPerColumn = static_cast<int>(ImGui::GetContentRegionAvail().x / 100);
	if (t_filesPerColumn <= 0)
	{
		t_filesPerColumn = 1;
	}
	if (ImGui::BeginTable("FileInfo", t_filesPerColumn))
	{
		int t_column = 1;
		int t_row = 1;
		std::filesystem::path t_mainPath = m_currentPath;
		for (const auto& t_entry : std::filesystem::directory_iterator(t_mainPath))
		{
			ImGui::TableNextColumn();
			if (t_column>t_filesPerColumn)
			{
				t_column = 1;
				t_row++;
			}
			const auto& t_path = t_entry.path();
			std::string t_filename = t_path.filename().string();
			ImGui::BeginChild((std::to_string(t_row) + std::to_string(t_column)).c_str(), ImVec2{ 100,100 });
			if (IsImage(t_path)) //if image, create a drag and drop source which carries the image's path
			{
				AddImageDragDropSource(t_path);
			}
			DrawFileImage(t_entry);
			DrawTextCentered(t_filename);
			ImGui::EndChild();
			if (t_entry.is_directory() && ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_currentPath = t_path;
			}
			t_column++;
			
		}
		ImGui::EndTable();
	}
}

void Editor::EditorLayer::Ui::FileExplorerWindow::DrawFileTree()
{
	std::string t_filename = m_rootPath.filename().string();
	if (HasChildDirectories(m_rootPath))
	{
		bool open = ImGui::TreeNodeEx(("##" + t_filename).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			m_currentPath = m_rootPath;
		}
		ImGui::SameLine();
		m_folderTexture->DrawTexture({ 15.f, 15.f });
		ImGui::SameLine();
		ImGui::Text(t_filename.c_str());
		if (open)
		{
			DrawFileTreeRecursive(m_rootPath);
			ImGui::TreePop();
		}

	}
	else
	{
		ImGui::TreeNodeEx(("##" + t_filename).c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth);
		if (ImGui::IsItemClicked())
		{
			m_currentPath = m_rootPath;
		}
		ImGui::SameLine();
		m_folderTexture->DrawTexture({ 15.f, 15.f });
		ImGui::SameLine();
		ImGui::Text(t_filename.c_str());

	}
}

void Editor::EditorLayer::Ui::FileExplorerWindow::DrawFileTreeRecursive(const std::filesystem::path& a_path)
{
	for (const auto& t_entry : std::filesystem::directory_iterator(a_path))
	{
		const auto& t_path = t_entry.path();
		std::string t_filename = t_path.filename().string();
		if (t_entry.is_directory()) {
			if (HasChildDirectories(t_path))
			{
				bool open = ImGui::TreeNodeEx(("##" + t_filename).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					m_currentPath = t_path;
				}
				ImGui::SameLine();
				m_folderTexture->DrawTexture({ 15.f, 15.f });
				ImGui::SameLine();
				ImGui::Text(t_filename.c_str());
				if (open)
				{
					DrawFileTreeRecursive(t_path);
					ImGui::TreePop();
				}
				
			}
			else
			{
				ImGui::TreeNodeEx(("##" + t_filename).c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth);
				if (ImGui::IsItemClicked())
				{
					m_currentPath = t_path;
				}
				ImGui::SameLine();
				m_folderTexture->DrawTexture({ 15.f, 15.f });
				ImGui::SameLine();
				ImGui::Text(t_filename.c_str());
				
			}
			
		}
	}
}

bool Editor::EditorLayer::Ui::FileExplorerWindow::HasChildDirectories(const std::filesystem::path& a_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(a_path))
	{
		if (entry.is_directory())
		{
			return true;
		}
	}
	return false;
}

bool Editor::EditorLayer::Ui::FileExplorerWindow::IsImage(const std::filesystem::path& a_path)
{
	std::string t_extension = a_path.extension().string();
	std::ranges::transform(t_extension, t_extension.begin(), ::tolower);
	return (t_extension == ".png" || t_extension == ".jpg" || t_extension == ".jpeg" || t_extension == ".bmp");
}

/**
 * Truncates the path specified in a_path to the root path.
 * For example, if we start with a path that is
 * "C:\School\Git\2024_gp_2028_gp_2028_projet_moteur-thecoolerengine\Assets\Textures\BaseObjectTexture.png"
 * This function will return
 * "Assets\Textures\BaseObjectTexture.png"
 */
std::filesystem::path Editor::EditorLayer::Ui::FileExplorerWindow::TruncatePathToRoot(const std::filesystem::path& a_path)
{
	return std::filesystem::relative(a_path, m_rootPath.parent_path());
}

void Editor::EditorLayer::Ui::FileExplorerWindow::DrawTextCentered(std::string a_text)
{
	float t_width = ImGui::GetContentRegionAvail().x;
	float t_offsetx = std::max((t_width - ImGui::CalcTextSize(a_text.c_str()).x) * 0.5f, 0.5f);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX()+t_offsetx);
	ImGui::TextUnformatted(a_text.c_str());
}

/**
 * Draws the icon of the type of file given in a_path
 */
void Editor::EditorLayer::Ui::FileExplorerWindow::DrawFileImage(const std::filesystem::directory_entry& a_path)
{
	if (a_path.is_directory())
		m_folderTexture->DrawTexture({ 50.f, 50.f }, true);
	else if (IsImage(a_path.path()))
		m_imageTexture->DrawTexture({ 50.f, 50.f }, true);
	else
		m_fileTexture->DrawTexture({ 50.f, 50.f }, true);
}

void Editor::EditorLayer::Ui::FileExplorerWindow::AddImageDragDropSource(const std::filesystem::path& a_path)
{
	if (ImGui::BeginDragDropSource())
	{
		std::filesystem::path t_path = TruncatePathToRoot(a_path);
		ImGui::SetDragDropPayload("IMAGE_PATH_PAYLOAD", t_path.string().c_str(), t_path.string().size() + 1);
		ImGui::Text(a_path.filename().string().c_str());
		ImGui::EndDragDropSource();
	}
}