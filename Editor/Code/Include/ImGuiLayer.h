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

class ImGuiLayer
{
public:
	static void Init(IWindow* window, Renderer* renderer);
	static void Update();
	static void Destroy();

	static void NewFrame();
	static void ImGuiDraw();
	static void Render();

	static void Start(const char* name);
	static void End();

private:
	static std::unique_ptr<RHIImGuiRenderer> m_ImGuiRenderer;

	static std::unique_ptr<MainWindow> m_MainWindow;
	static std::vector<std::shared_ptr<SubWindow>> m_SubWindows;
};
