#ifndef SUBWINDOWS_H
#define SUBWINDOWS_H

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

class MainWindow;

class SubWindow
{
public:

	virtual ~SubWindow() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;
protected:
	SubWindow();
private:

	std::string name;

	//ImGUI info
	ImGuiID id;

	//Window hierarchy info
	MainWindow* mainWindow;
	std::vector<std::shared_ptr<SubWindow>> subWindows;
};

#endif