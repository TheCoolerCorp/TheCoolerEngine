#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include "EngineExport.h"
#include "Core/Window/IWindow.h"
#include "GamePlay/Components/Component.h"
#include "nlohmann/json.hpp"


namespace Engine::Core::Window
{
	class IInputHandler;
}
namespace Engine::GamePlay
{
	class Scene;

	class GameComponent : public Component
	{
	public:
		ENGINE_API virtual void Create(int& a_outId) = 0;
		ENGINE_API virtual void Destroy() = 0;

		ENGINE_API virtual nlohmann::ordered_json Serialize() = 0;
		ENGINE_API virtual void Deserialize(const nlohmann::ordered_json& a_json) = 0;

		ENGINE_API virtual std::string GetTypeName() const { return ""; }

		ENGINE_API virtual void DrawUI() {}

		ENGINE_API virtual void Start() {}
		ENGINE_API virtual void Update(float a_deltatime) {}
		ENGINE_API virtual void SceneUpdate() {}
		ENGINE_API virtual void ProcessInputs(Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) {}

		ENGINE_API void SetScene(Scene* a_scene) { m_scene = a_scene; }
		ENGINE_API void SetWindow(Core::Window::IWindow* a_window) { m_window = a_window; }
	protected:
		Scene* m_scene = nullptr;

		/**
		 * Tells the IWindow to capture the cursor.
		 * This function allows the child classes to capture the cursor when needed,
		 * without having access to the IWindow directly.
		 * @param a_capture bool to say if you want to capture the cursor or not
		 */
		void CaptureCursor(bool a_capture = true) const
		{
			if (m_window)
			{
				m_window->CaptureCursor(a_capture);
			}
		}
	private:
		Core::Window::IWindow* m_window = nullptr;
	};
}

#endif // !GAMECOMPONENT_H
