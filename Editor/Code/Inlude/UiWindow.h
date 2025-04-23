#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <Core/Renderer/Renderer.h>

namespace Engine::Core
{
	class Renderer;
}

namespace Editor::EditorLayer::Ui
{
	class ImGuiLayer;

	/*
	* Ui Window base class. Acts as a container for any ImGui window.
	*/
	class UiWindow
	{
	public:
		UiWindow(Engine::Core::Renderer* a_renderer, ImGuiLayer* layer)
			: m_renderer(a_renderer), m_layer(layer) {}
		virtual ~UiWindow() = default;
		virtual void Create() = 0;
		virtual void UiDraw() = 0;
		virtual void Destroy() = 0;
	protected:
		std::string m_name;

		Engine::Core::Renderer* m_renderer;
		Editor::EditorLayer::Ui::ImGuiLayer* m_layer;
	};
}

#endif