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


class ImGuiLayer
{
public:
	static void Init();
	static void Update();
	static void Destroy();

	static void NewFrame();
	static void Render();

	static void Start(const char* name);
	static void End();

private:
	static std::unique_ptr<MainWindow> m_MainWindow;
	static std::vector<std::shared_ptr<SubWindow>> m_SubWindows;
};
