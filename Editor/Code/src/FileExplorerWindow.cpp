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
	SetCurrentPath(m_rootPath);
}

void Editor::EditorLayer::Ui::FileExplorerWindow::UiDraw()
{
	//No padding, so that the windows dont take up more space than they need
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::Begin((m_name + "##" + std::to_string(m_uid)).c_str(), &m_open);

	float t_width = ImGui::GetContentRegionAvail().x*0.3f;
	t_width = std::min(t_width, 150.f);

	/*
	 * The first child draws the file tree, which is a tree of all the folders in the root path.
	 */
	ImGui::BeginChild(("FileTree" + std::to_string(m_uid)).c_str(),ImVec2(t_width, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PopStyleVar(2);

	UiDrawFileTree();
	ImGui::EndChild();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SameLine();

	/*
	 * The second child draws the file info, which is a table of all the files in the current path, with drag & drop functionality for certain file types.
	 * It also draws a toolbar with a back button, a preview button to toggle the preview of images in the file explorer, and the current path.
	 */
	ImGui::BeginChild(("FileInfo" + std::to_string(m_uid)).c_str(), ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::BeginChild(("Toolbar" + std::to_string(m_uid)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 20.0f), ImGuiChildFlags_Borders);
	ImGui::PopStyleVar(2);
	if (ImGui::Button(("Back##" + std::to_string(m_uid)).c_str()))
	{
		if (m_currentPath.has_parent_path() && m_currentPath != m_rootPath)
		{
			SetCurrentPath(m_currentPath.parent_path());
		}
	}
	ImGui::SameLine();
	if(ImGui::Checkbox(("Preview##"+std::to_string(m_uid)).c_str(), &m_previewImages))
	{
		if (m_previewImages)
		{
			LoadContextImages(m_currentPath);
		}
		else
		{
			ClearContextImages();
		}
	}
	ImGui::SetItemTooltip("Makes the images preview in the file explorer. \n !!WARNING!! : Loading is done in the main thread and, as such, the program will freeze loading a lot of images for the first time \n Once an image has been loaded, there will no longer be an issue");
	ImGui::SameLine();
	ImGui::Text(m_currentPath.string().c_str());
	ImGui::EndChild();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::BeginChild(("Browser" + std::to_string(m_uid)).c_str(), ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PopStyleVar(2);

	UiDrawFileInfo();
	ImGui::EndChild();

	ImGui::EndChild();

	ImGui::End();
}

void Editor::EditorLayer::Ui::FileExplorerWindow::ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
{
}

void Editor::EditorLayer::Ui::FileExplorerWindow::Destroy()
{
	ClearContextImages();
	if (m_folderTexture)
	{
		m_folderTexture->Destroy();
		delete m_folderTexture;
		m_folderTexture = nullptr;
	}
	if (m_fileTexture)
	{
		m_fileTexture->Destroy();
		delete m_fileTexture;
		m_fileTexture = nullptr;
	}
	if (m_imageTexture)
	{
		m_imageTexture->Destroy();
		delete m_imageTexture;
		m_imageTexture = nullptr;
	}
}

void Editor::EditorLayer::Ui::FileExplorerWindow::NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object)
{
}

/**
 * Sets the current active path, used by UiDrawFileInfo to draw the content of the path as a table,
 * to the one given in a_path
 */
void Editor::EditorLayer::Ui::FileExplorerWindow::SetCurrentPath(std::filesystem::path a_path)
{
	m_currentPath = a_path;
	if (m_previewImages)
	{
		LoadContextImages(m_currentPath);
	}
}

/**
 * Draws the content of m_currentPath (the currently selected folder) in a ImGui table
 * With an image illustrating its type and its name. Folders can be double clicked to acess its child
 * A drag and drop source is created for each image and model
 */
void Editor::EditorLayer::Ui::FileExplorerWindow::UiDrawFileInfo()
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
				UiAddImageDragDropSource(t_path);
			}
			else if (IsModel(t_path)) //if model, create a drag and drop source which carries the model's path
			{
				UiAddModelDragDropSource(t_path);
			}
			UiDrawFileImage(t_entry);
			UiDrawTextCentered(t_filename);
			ImGui::EndChild();
			if (t_entry.is_directory() && ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				SetCurrentPath(t_path);
			}
			t_column++;
			
		}
		ImGui::EndTable();
	}
}

/**
 * Draws the file tree, which is a tree of all the folders in the root path.
 * If any of the elements is selected, it will set the current path to the selected folder.
 * If the folder has no child folders, it will be drawn as a leaf node.
 */
void Editor::EditorLayer::Ui::FileExplorerWindow::UiDrawFileTree()
{
	std::string t_filename = m_rootPath.filename().string();
	if (HasChildDirectories(m_rootPath))
	{
		bool open = ImGui::TreeNodeEx(("##" + t_filename).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			SetCurrentPath(m_rootPath);
		}
		ImGui::SameLine();
		m_folderTexture->DrawTexture({ 15.f, 15.f });
		ImGui::SameLine();
		ImGui::Text(t_filename.c_str());
		if (open)
		{
			UiDrawFileTreeRecursive(m_rootPath);
			ImGui::TreePop();
		}

	}
	else
	{
		ImGui::TreeNodeEx(("##" + t_filename).c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth);
		if (ImGui::IsItemClicked())
		{
			SetCurrentPath(m_rootPath);
		}
		ImGui::SameLine();
		m_folderTexture->DrawTexture({ 15.f, 15.f });
		ImGui::SameLine();
		ImGui::Text(t_filename.c_str());

	}
}

void Editor::EditorLayer::Ui::FileExplorerWindow::UiDrawFileTreeRecursive(const std::filesystem::path& a_path)
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
					SetCurrentPath(t_path);
				}
				ImGui::SameLine();
				m_folderTexture->DrawTexture({ 15.f, 15.f });
				ImGui::SameLine();
				ImGui::Text(t_filename.c_str());
				if (open)
				{
					UiDrawFileTreeRecursive(t_path);
					ImGui::TreePop();
				}
				
			}
			else
			{
				ImGui::TreeNodeEx(("##" + t_filename).c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth);
				if (ImGui::IsItemClicked())
				{
					SetCurrentPath(t_path);
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

bool Editor::EditorLayer::Ui::FileExplorerWindow::IsModel(const std::filesystem::path& a_path)
{
	std::string t_extension = a_path.extension().string();
	std::ranges::transform(t_extension, t_extension.begin(), ::tolower);
	return t_extension == ".obj"; //only .obj is supported for now
	//return (t_extension == ".fbx" || t_extension == ".obj" || t_extension == ".gltf" || t_extension == ".glb");
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


/**
 * Loads all images in the given path and stores them in m_textures
 * Deletes all previous images in m_textures beforehand.
 */
void Editor::EditorLayer::Ui::FileExplorerWindow::LoadContextImages(const std::filesystem::path& a_path)
{
	ClearContextImages();
	for (const auto& entry : std::filesystem::directory_iterator(a_path))
	{
		if (entry.is_regular_file())
		{
			std::string t_filename = entry.path().filename().string();
			std::filesystem::path t_path = entry.path();
			if (IsImage(entry.path()))
			{
				m_textures[t_filename] = new ImGuiTexture(m_renderer, TruncatePathToRoot(t_path).string());
			}
		}
	}
}

void Editor::EditorLayer::Ui::FileExplorerWindow::ClearContextImages()
{
	vkDeviceWaitIdle(m_renderer->GetLogicalDevice()->CastVulkan()->GetVkDevice());
	for (auto& t_texture : m_textures)
	{
		t_texture.second->Destroy();
		delete t_texture.second;
	}
	m_textures.clear();
}

void Editor::EditorLayer::Ui::FileExplorerWindow::UiDrawTextCentered(std::string a_text)
{
	float t_width = ImGui::GetContentRegionAvail().x;
	float t_offsetx = std::max((t_width - ImGui::CalcTextSize(a_text.c_str()).x) * 0.5f, 0.5f);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX()+t_offsetx);
	ImGui::TextUnformatted(a_text.c_str());
}

/**
 * Draws the icon of the type of file given in a_path
 */
void Editor::EditorLayer::Ui::FileExplorerWindow::UiDrawFileImage(const std::filesystem::directory_entry& a_path)
{
	if (a_path.is_directory())
	{
		m_folderTexture->DrawTexture({ 50.f, 50.f }, true);
	}
	else if (IsImage(a_path.path()))
	{
		if (m_textures.find(a_path.path().filename().string()) != m_textures.end())
		{
			m_textures[a_path.path().filename().string()]->DrawTexture({ 50.f, 50.f }, true);
		}
		else
		{
			m_imageTexture->DrawTexture({ 50.f, 50.f }, true);
		}
		
	}
	else
	{
		m_fileTexture->DrawTexture({ 50.f, 50.f }, true);
	}
}

void Editor::EditorLayer::Ui::FileExplorerWindow::UiAddImageDragDropSource(const std::filesystem::path& a_path)
{
	if (ImGui::BeginDragDropSource())
	{
		std::filesystem::path t_path = TruncatePathToRoot(a_path);
		ImGui::SetDragDropPayload("IMAGE_PATH_PAYLOAD", t_path.string().c_str(), t_path.string().size() + 1);
		ImGui::Text(a_path.filename().string().c_str());
		ImGui::EndDragDropSource();
	}
}

void Editor::EditorLayer::Ui::FileExplorerWindow::UiAddModelDragDropSource(const std::filesystem::path& a_path)
{
	if (ImGui::BeginDragDropSource())
	{
		std::filesystem::path t_path = TruncatePathToRoot(a_path);
		ImGui::SetDragDropPayload("MESH_PATH_PAYLOAD", t_path.string().c_str(), t_path.string().size() + 1);
		ImGui::Text(a_path.filename().string().c_str());
		ImGui::EndDragDropSource();
	}
}
