#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <Core/Renderer/Renderer.h>

#include "GamePlay/Others/GameObject.h"

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

		virtual void NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object) = 0;

		void SetName(std::string a_name) { m_name = std::move(a_name); }
		void SetUid(int a_uid) { m_uid = a_uid; }

		[[nodiscard]] int GetUid() { return m_uid; }
		[[nodiscard]] const std::string& GetName() const { return m_name; }
		[[nodiscard]] bool IsClosed() const { return !m_open; }
		[[nodiscard]] Engine::Core::Renderer* GetRenderer() const { return m_renderer; }
	protected:
		std::string m_name;
		int m_uid;

		//if false, the window will be closed and deleted during the layers update call
		bool m_open = true;

		Engine::Core::Renderer* m_renderer;
		ImGuiLayer* m_layer;
	};
}

#endif