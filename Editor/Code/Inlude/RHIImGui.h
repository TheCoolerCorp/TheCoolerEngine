#ifndef RHIIMGUI_H
#define RHIIMGUI_H

class ImGuiLayer;

namespace Engine::Core
{
	namespace Window
	{
		class IWindow;
	}
	class Renderer;
}

class RHIImGui
{
public:
	RHIImGui(Engine::Core::Renderer* renderer) { m_renderer = renderer;  }
	virtual ~RHIImGui() = default;

	virtual void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer) = 0;

	virtual void NewFrame() = 0;
	virtual void Render() = 0;
	virtual void DrawSceneAsImage() = 0;


	virtual void Cleanup() = 0;

	void SetImGuiParent(ImGuiLayer* imguiLayer) { m_imguiLayer = imguiLayer; }
protected:
	Engine::Core::Renderer* m_renderer;
	ImGuiLayer* m_imguiLayer = nullptr;
};

#endif