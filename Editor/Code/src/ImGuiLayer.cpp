#include "ImGuiLayer.h"

#include "FileExplorerWindow.h"
#include "VulkanImGui.h"
#include "Core/Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "LoggerWindow.h"
#include "../Include/Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "../Include/UiWindow.h"
#include "../Include/SceneGraphWindow.h"
#include "../Include/InspectorWindow.h"
#include "Math/TheCoolerMath.h"
#include "Core/Window/IInputHandler.h"

namespace Editor::EditorLayer::Ui
{

	void ImGuiLayer::OnAttach(Engine::Core::Window::IWindow* a_window)
	{
		Layer::OnAttach(a_window);
		m_imGui = new VulkanImGui(m_renderer);
		m_imGui->SetImGuiParent(this);
		m_imGui->Init(a_window, m_renderer);
		SetupImGuiStyle();
	}

	void ImGuiLayer::OnDetach()
	{

		Layer::OnDetach();
	}

	void ImGuiLayer::OnUpdate(float a_deltaTime)
	{
		Layer::OnUpdate(a_deltaTime);
		
		m_imGui->Update();
		std::vector<int> t_indexesToDelete;
		for (UiWindow* t_window : m_windows)
		{
			if (t_window == nullptr)
				continue;
			if (t_window->IsClosed())
			{
				t_indexesToDelete.push_back(t_window->GetUid());
			}
		}
		for (int t_index : t_indexesToDelete)
		{
			UiWindow* t_window = m_windows[t_index];
			t_window->Destroy();
			delete t_window;
			m_windows[t_index] = nullptr;
			m_availableIds.push_back(t_index);
			break;
		}
		OnUiRender();
	}

	void ImGuiLayer::OnUiRender()
	{
		Layer::OnUiRender();
		m_imGui->NewFrame();
		GizmoBeginFrame();

		UiSetupDockspace();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f,0.0f });

		//Main viewport
		UiDrawViewport();
		
		//Extra windows
		UiDrawWindows();

		m_imGui->Render();

	}

	void ImGuiLayer::OnProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
	{
		Layer::OnProcessInputs(a_inputHandler, a_deltaTime);
		for (UiWindow* t_window : m_windows)
		{
			if (t_window == nullptr)
				continue;
			t_window->ProcessInputs(a_inputHandler, a_deltaTime);
		}
		ImGuiWindow* t_viewPortWindow = ImGui::FindWindowByName("Viewport");
		//if the viewport window is focused, process inputs
		if (t_viewPortWindow && GImGui->HoveredWindow == t_viewPortWindow)
		{
			if (a_inputHandler->IsKeyDown(Engine::Core::Window::Key::KEY_G))
			{
				GizmoSetCurrentOperation(ImGuizmo::TRANSLATE);
			}
			if (a_inputHandler->IsKeyDown(Engine::Core::Window::Key::KEY_R))
			{
				GizmoSetCurrentOperation(ImGuizmo::ROTATE);
			}
			if (a_inputHandler->IsKeyDown(Engine::Core::Window::Key::KEY_E))
			{
				GizmoSetCurrentOperation(ImGuizmo::SCALE);
			}
		}
	}

	void ImGuiLayer::Delete()
	{
		//delete every UiWindow
		for (UiWindow* t_window : m_windows)
		{
			if (!t_window)
				continue;
			t_window->Destroy();
			delete t_window;
		}
		if (m_imGui)
		{
			//m_imGui->Cleanup();
			delete m_imGui;
			m_imGui = nullptr;
		}
		Layer::OnDetach();
	}

	void ImGuiLayer::AddWindow(UiWindow* a_window)
	{
		if (a_window == nullptr)
			return;
		if (!m_availableIds.empty())
		{
			int id = m_availableIds.back();
			m_availableIds.pop_back();
			m_windows[id] = a_window;
			a_window->SetUid(static_cast<int>(id));
		}
		else
		{
			m_windows.push_back(a_window);
			a_window->Create();
			a_window->SetUid(static_cast<int>(m_windows.size()) - 1);
		}
	}

	void ImGuiLayer::DeselectObject()
	{
		if (m_selectedGameObject)
		{
			m_selectedGameObject = nullptr;
		}
	}

	/**
	 * Notifies all windows that a specified GameObject has been removed
	 * This is to prevent any active window trying to access a GameObject that has been deleted
	 * if they are storing a reference to it
	 */
	void ImGuiLayer::NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object)
	{
		if (m_selectedGameObject)
		{
			if (a_object->GetId() == m_selectedGameObject->GetId())
				m_selectedGameObject = nullptr;
		}
		for (UiWindow* t_window : m_windows)
		{
			if (t_window == nullptr)
				continue;
			t_window->NotifyObjectRemoved(a_object);
		}
	}

	void ImGuiLayer::UiDrawItemAddMenu()
	{
		if (ImGui::BeginMenu("Add Object"))
		{
			if (ImGui::MenuItem("Empty Object"))
			{
				SetSelectedGameObject(m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_EMPTY));
			}
			if (ImGui::MenuItem("Camera"))
			{
				SetSelectedGameObject(m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_CAMERA));
			}
			if (ImGui::MenuItem("Cube"))
			{
				SetSelectedGameObject(m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_CUBE));
			}
			if (ImGui::MenuItem("Sphere"))
			{
				SetSelectedGameObject(m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_SPHERE));
			}
			if (ImGui::MenuItem("Plane"))
			{
				SetSelectedGameObject(m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_PLANE));
			}
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Point Light"))
				{
					SetSelectedGameObject(m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_LIGHT));
				}
				if (ImGui::MenuItem("Directional Light"))
				{
					//m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_LIGHT);
				}
				if (ImGui::MenuItem("Spot Light"))
				{
					//m_app->GetCurrentScene()->AddGameObject(Engine::GamePlay::GameObjectType::OBJECTTYPE_LIGHT);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
	}

	void ImGuiLayer::GizmoBeginFrame()
	{
		ImGuizmo::SetOrthographic(false); // perspective or ortho
		ImGuizmo::BeginFrame();

	}

	void ImGuiLayer::GizmoMainDraw()
	{
		Engine::Math::mat4 t_identityMatrix = Engine::Math::mat4(1.0f);
		Engine::Math::mat4 t_cameraView = m_app->GetCurrentScene()->GetMainCamera()->GetViewMatrix();
		Engine::Math::mat4 t_cameraProjection = m_app->GetCurrentScene()->GetMainCamera()->GetProjectionMatrix();

		t_cameraProjection.mElements[5] *= -1.f; // flip the Y axis for the projection matrix

		t_identityMatrix.Transpose();
		t_cameraView.Transpose();
		t_cameraProjection.Transpose();


		const float* t_viewf = t_cameraView.mElements.data();
		const float* t_projectionf = t_cameraProjection.mElements.data();

		ImGuizmo::SetDrawlist();
		ImVec2 imageMin = ImGui::GetItemRectMin();
		ImVec2 imageSize = ImGui::GetItemRectSize();
		ImGuizmo::SetRect(imageMin.x, imageMin.y, imageSize.x, imageSize.y);

		if (m_selectedGameObject && m_selectedGameObject->GetComponent<Engine::GamePlay::TransformComponent>())
		{
			Engine::Math::mat4 t_objectMatrix = m_selectedGameObject->GetComponent<Engine::GamePlay::TransformComponent>()->GetMatrix();
			t_objectMatrix.Transpose();

			if (ImGuizmo::Manipulate(t_viewf, t_projectionf, m_currentGizmoOperation, m_currentGizmoMode, t_objectMatrix.mElements.data()))
			{
				float t_translation[3], t_rotation[3], t_scale[3];

				ImGuizmo::DecomposeMatrixToComponents(t_objectMatrix.mElements.data(), t_translation, t_rotation, t_scale);
				Engine::GamePlay::TransformData t_transformData;

				t_transformData.mRot = Engine::Math::quat(Engine::Math::vec3(t_rotation[0]* Engine::Math::PI/180.0f, t_rotation[1] * Engine::Math::PI / 180.0f, t_rotation[2] * Engine::Math::PI / 180.0f));
				t_transformData.mRot = Engine::Math::quat::Normalize(t_transformData.mRot);

				t_transformData.mPos = Engine::Math::vec3(t_translation[0], t_translation[1], t_translation[2]);
				t_transformData.mScale = Engine::Math::vec3(t_scale[0], t_scale[1], t_scale[2]);

				t_transformData.mParentId = m_selectedGameObject->GetComponent<Engine::GamePlay::TransformComponent>()->GetParentID();

				m_selectedGameObject->GetComponent<Engine::GamePlay::TransformComponent>()->Set(t_transformData);
			}
		}
	}

	void ImGuiLayer::SetupImGuiStyle()
	{

		// Dark Ruda style by Raikiri from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6000000238418579f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 4.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 14.0f;
		style.ScrollbarRounding = 9.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 4.0f;
		style.TabRounding = 4.0f;
		style.TabBorderSize = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ToSrgb(ImVec4(0.9490196108818054f, 0.95686274766922f, 0.9764705896377563f, 1.0f));
		style.Colors[ImGuiCol_TextDisabled] = ToSrgb(ImVec4(0.3568627536296844f, 0.4196078479290009f, 0.4666666686534882f, 1.0f));
		style.Colors[ImGuiCol_WindowBg] = ToSrgb(ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f));
		style.Colors[ImGuiCol_ChildBg] = ToSrgb(ImVec4(0.1490196138620377f, 0.1764705926179886f, 0.2196078449487686f, 1.0f));
		style.Colors[ImGuiCol_PopupBg] = ToSrgb(ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f));
		style.Colors[ImGuiCol_Border] = ToSrgb(ImVec4(0.0784313753247261f, 0.09803921729326248f, 0.1176470592617989f, 1.0f));
		style.Colors[ImGuiCol_BorderShadow] = ToSrgb(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		style.Colors[ImGuiCol_FrameBg] = ToSrgb(ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f));
		style.Colors[ImGuiCol_FrameBgHovered] = ToSrgb(ImVec4(0.1176470592617989f, 0.2000000029802322f, 0.2784313857555389f, 1.0f));
		style.Colors[ImGuiCol_FrameBgActive] = ToSrgb(ImVec4(0.08627451211214066f, 0.1176470592617989f, 0.1372549086809158f, 1.0f));
		style.Colors[ImGuiCol_TitleBg] = ToSrgb(ImVec4(0.08627451211214066f, 0.1176470592617989f, 0.1372549086809158f, 0.6499999761581421f));
		style.Colors[ImGuiCol_TitleBgActive] = ToSrgb(ImVec4(0.0784313753247261f, 0.09803921729326248f, 0.1176470592617989f, 1.0f));
		style.Colors[ImGuiCol_TitleBgCollapsed] = ToSrgb(ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f));
		style.Colors[ImGuiCol_MenuBarBg] = ToSrgb(ImVec4(0.1490196138620377f, 0.1764705926179886f, 0.2196078449487686f, 1.0f));
		style.Colors[ImGuiCol_ScrollbarBg] = ToSrgb(ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.3899999856948853f));
		style.Colors[ImGuiCol_ScrollbarGrab] = ToSrgb(ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f));
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ToSrgb(ImVec4(0.1764705926179886f, 0.2196078449487686f, 0.2470588237047195f, 1.0f));
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ToSrgb(ImVec4(0.08627451211214066f, 0.2078431397676468f, 0.3098039329051971f, 1.0f));
		style.Colors[ImGuiCol_CheckMark] = ToSrgb(ImVec4(0.2784313857555389f, 0.5568627715110779f, 1.0f, 1.0f));
		style.Colors[ImGuiCol_SliderGrab] = ToSrgb(ImVec4(0.2784313857555389f, 0.5568627715110779f, 1.0f, 1.0f));
		style.Colors[ImGuiCol_SliderGrabActive] = ToSrgb(ImVec4(0.3686274588108063f, 0.6078431606292725f, 1.0f, 1.0f));
		style.Colors[ImGuiCol_Button] = ToSrgb(ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f));
		style.Colors[ImGuiCol_ButtonHovered] = ToSrgb(ImVec4(0.2784313857555389f, 0.5568627715110779f, 1.0f, 1.0f));
		style.Colors[ImGuiCol_ButtonActive] = ToSrgb(ImVec4(0.05882352963089943f, 0.529411792755127f, 0.9764705896377563f, 1.0f));
		style.Colors[ImGuiCol_Header] = ToSrgb(ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 0.550000011920929f));
		style.Colors[ImGuiCol_HeaderHovered] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f));
		style.Colors[ImGuiCol_HeaderActive] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f));
		style.Colors[ImGuiCol_Separator] = ToSrgb(ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f));
		style.Colors[ImGuiCol_SeparatorHovered] = ToSrgb(ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f));
		style.Colors[ImGuiCol_SeparatorActive] = ToSrgb(ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f));
		style.Colors[ImGuiCol_ResizeGrip] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.25f));
		style.Colors[ImGuiCol_ResizeGripHovered] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f));
		style.Colors[ImGuiCol_ResizeGripActive] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f));
		style.Colors[ImGuiCol_Tab] = ToSrgb(ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f));
		style.Colors[ImGuiCol_TabHovered] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f));
		style.Colors[ImGuiCol_TabActive] = ToSrgb(ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f));
		style.Colors[ImGuiCol_TabUnfocused] = ToSrgb(ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f));
		style.Colors[ImGuiCol_TabUnfocusedActive] = ToSrgb(ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f));
		style.Colors[ImGuiCol_PlotLines] = ToSrgb(ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f));
		style.Colors[ImGuiCol_PlotLinesHovered] = ToSrgb(ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f));
		style.Colors[ImGuiCol_PlotHistogram] = ToSrgb(ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f));
		style.Colors[ImGuiCol_PlotHistogramHovered] = ToSrgb(ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f));
		style.Colors[ImGuiCol_TableHeaderBg] = ToSrgb(ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f));
		style.Colors[ImGuiCol_TableBorderStrong] = ToSrgb(ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f));
		style.Colors[ImGuiCol_TableBorderLight] = ToSrgb(ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f));
		style.Colors[ImGuiCol_TableRowBg] = ToSrgb(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		style.Colors[ImGuiCol_TableRowBgAlt] = ToSrgb(ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f));
		style.Colors[ImGuiCol_TextSelectedBg] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f));
		style.Colors[ImGuiCol_DragDropTarget] = ToSrgb(ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f));
		style.Colors[ImGuiCol_NavHighlight] = ToSrgb(ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f));
		style.Colors[ImGuiCol_NavWindowingHighlight] = ToSrgb(ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f));
		style.Colors[ImGuiCol_NavWindowingDimBg] = ToSrgb(ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f));
		style.Colors[ImGuiCol_ModalWindowDimBg] = ToSrgb(ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f));
	
	}

	/**
	 * Workaround to imgui not supporting srgb, converts the given linear space colour to srgb and returs it
	 * Not perfect, but its a very easy solution with an acceptable margin of error
	 * and no performance cost
	 */
	ImVec4 ImGuiLayer::ToSrgb(ImVec4 rgba)
	{
		float r = powf(rgba.x, 2.2f);
		float g = powf(rgba.y, 2.2f);
		float b = powf(rgba.z, 2.2f);
		return ImVec4(r, g, b, rgba.w);
	}

	void ImGuiLayer::UiSetupDockspace()
	{
		bool dockSpaceOpen = true;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		//Dockspace setup
		ImGui::Begin("DockSpace", &dockSpaceOpen, window_flags);
		ImGuiID t_dockSpaceID = ImGui::GetID("MainDockSpace");
		ImGuiDockNodeFlags t_dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGui::DockSpace(t_dockSpaceID, ImVec2(0.0f, 0.0f), t_dockspaceFlags);
		ImGui::PopStyleVar(2);

		//Menu bar setup
		UiDrawMenuBar();
		ImGui::End();
	}


	void ImGuiLayer::UiDrawMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::BeginMenu("Add Window"))
				{
					if (ImGui::MenuItem("Logger"))
					{
						UiLoggerWindow* t_window = new UiLoggerWindow(m_renderer, this);
						t_window->SetName("Logger");
						AddWindow(t_window);
					}
					if (ImGui::MenuItem("Scene Graph"))
					{
						SceneGraphUiWindow* t_window = new SceneGraphUiWindow(m_renderer, this, m_app->GetCurrentScene());
						t_window->SetName("Scene Graph");
						AddWindow(t_window);
					}
					if (ImGui::MenuItem("Inspector"))
					{
						InspectorUiWindow* t_window = new InspectorUiWindow(m_renderer, this);
						t_window->SetName("Inspector");
						AddWindow(t_window);
					}
					if (ImGui::MenuItem("Explorer"))
					{
						FileExplorerWindow* t_window = new FileExplorerWindow(m_renderer, this);
						t_window->SetName("File Explorer");
						AddWindow(t_window);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Save Scene"))//placeholder for when this gets implemented
				{
					//m_app->GetCurrentScene()->SaveScene();
				}
				if (ImGui::MenuItem("Load Scene"))
				{
					//m_app->GetCurrentScene()->LoadScene();
				}
				ImGui::Separator();
				UiDrawItemAddMenu();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void ImGuiLayer::UiDrawViewport()
	{
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		m_imGui->DrawSceneAsImage();

		//gizmo draw inside the viewport
		GizmoMainDraw();

		if (m_selectedGameObject != nullptr)
		{
			ImGui::SetCursorPos(ImVec2(10, 20));
		
			int t_currentMode;
			switch (m_currentGizmoOperation)
			{
			case ImGuizmo::TRANSLATE:
				t_currentMode = 0;
				break;
			case ImGuizmo::ROTATE:
				t_currentMode = 1;
				break;
			case ImGuizmo::SCALE:
				t_currentMode = 2;
				break;
			default:
				t_currentMode = 0;
			}

			int t_isLocal = 0;
			if (m_currentGizmoMode == ImGuizmo::WORLD)
			{
				t_isLocal = 1;
			}

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
			ImGui::BeginChild("GizmoMode", ImVec2(100.0f, ImGui::GetContentRegionAvail().y));

			//Switch between gizmo modes
			if(ImGui::RadioButton("Move", &t_currentMode, 0))
			{
				m_currentGizmoOperation = ImGuizmo::TRANSLATE;
			}
			if(ImGui::RadioButton("Rotate", &t_currentMode, 1))
			{
				m_currentGizmoOperation = ImGuizmo::ROTATE;
			}
			if(ImGui::RadioButton("Scale", &t_currentMode, 2))
			{
				m_currentGizmoOperation = ImGuizmo::SCALE;
			}
			ImGui::Separator();
			//Switch between local and global gizmo
			if (ImGui::RadioButton("Local", &t_isLocal, 0))
			{
				m_currentGizmoMode = ImGuizmo::LOCAL;
			}
			if (ImGui::RadioButton("Global", &t_isLocal, 1))
			{
				m_currentGizmoMode = ImGuizmo::WORLD;
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();
			
		}
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 60, 20));
		ImGui::BeginChild("PlayPause Button", ImVec2(140, 70));
		if (ImGui::Button("Play", ImVec2(60, ImGui::CalcTextSize("Play").y+10)))
		{
			
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause", ImVec2(60, ImGui::CalcTextSize("Pause").y + 10)))
		{

		}
		ImGui::PopStyleColor();
		ImGui::EndChild();
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ImGuiLayer::UiDrawWindows()
	{
		for (UiWindow* t_window : m_windows)
		{
			if (t_window == nullptr)
				continue;
			t_window->UiDraw();
		}
	}
}
