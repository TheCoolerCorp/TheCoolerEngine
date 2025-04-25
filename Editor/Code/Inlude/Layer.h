#ifndef LAYERBASE_H
#define LAYERBASE_H
#include <string>

namespace Editor::Core
{
	class Application;
}

namespace Engine::Core
{
	namespace Window
	{
		class IWindow;
	}

	class Renderer;
}
namespace Editor::EditorLayer
{
	/*
   * Layers are used to add functionality to the editor, like ui, audio, etc.
   * Each layer should be self-contained and should not depend on other layers.
   * Removing/Adding a layer should not affect the rest of the editor and not prevent
   * the application from running. It is allowed to communicate with the Engine but
   * the engine is not.
   */
	class Layer {
	public:
		Layer(Engine::Core::Renderer* a_renderer, Core::Application* a_app, std::string a_name = "UnnamedLayer")
		: m_app(a_app)
		, m_renderer(a_renderer)
		, m_name(std::move(a_name))
		{}
		virtual ~Layer() = default;

		// Called once when the layer is added
		virtual void OnAttach(Engine::Core::Window::IWindow* a_window) {}

		// Called once when the layer is removed
		virtual void OnDetach() {}

		// Called every frame (or editor tick)
		virtual void OnUpdate(float a_deltaTime) {}
		virtual void OnUiRender() {}

		virtual void Delete() = 0;

		//possible event handling later,,,, maybe
		//virtual void OnEvent(class Event& event) {}

		const std::string& GetName() const { return m_name; }

	protected:
		Core::Application* m_app = nullptr;
		Engine::Core::Renderer* m_renderer;
		std::string m_name;
	};
}

#endif