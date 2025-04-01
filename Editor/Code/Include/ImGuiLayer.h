#pragma once
//ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

//Standard includes
#include <vector>
#include <memory>
#include <string>

//Custom includes
#include "MainWindow.h"
#include "SubWindow.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Window/IWindow.h"
#include "RHIImGuiRenderer.h"
#include "Layer.h"
class ImGuiLayer : public Editor::Core::Layer
{
public:
	ImGuiLayer() = default;
	~ImGuiLayer() override;

	void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer) override;
	void Update() override;
	void DrawUpdate();
	void Destroy() override;

	static void NewFrame();
	static void ImGuiDraw();
	static void Render();

	static ImDrawData* GetDrawData();

	static void Start(const char* name);
	static void End();


private:
	static RHIImGuiRenderer* m_imGuiRenderer;

	std::unique_ptr<MainWindow> m_mainWindow;
	std::vector<std::shared_ptr<SubWindow>> m_subWindows;
};
