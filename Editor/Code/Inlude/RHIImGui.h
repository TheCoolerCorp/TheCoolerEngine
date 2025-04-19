#ifndef RHIIMGUI_H
#define RHIIMGUI_H

namespace Editor::EditorLayer::Ui
{
	class ImGuiLayer;
}

namespace Engine::Core
{
	namespace Window
	{
		class IWindow;
	}
	class Renderer;
}
namespace Editor::EditorLayer::Ui
{
	class RHIImGui
	{
	public:
		RHIImGui(Engine::Core::Renderer* a_renderer) { m_renderer = a_renderer;  }
		virtual ~RHIImGui() = default;

		virtual void Init(Engine::Core::Window::IWindow* a_window, Engine::Core::Renderer* a_renderer) = 0;

		virtual void NewFrame() = 0;
		virtual void Render() = 0;
		virtual void DrawSceneAsImage() = 0;


		virtual void Cleanup() = 0;

		void SetImGuiParent(ImGuiLayer* a_imguiLayer) { m_imguiLayer = a_imguiLayer; }
	protected:
		Engine::Core::Renderer* m_renderer = nullptr;
		ImGuiLayer* m_imguiLayer = nullptr;
	};
}

#endif