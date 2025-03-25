#ifndef GLNPUTHANDLER_H
#define GLNPUTHANDLER_H

#include "EngineExport.h"

#include "IInputHandler.h"
#include "GLWindow.h"
namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			class GLInputHandler : public IInputHandler
			{
			public:
				ENGINE_API ~GLInputHandler() override = default;

				ENGINE_API GLInputHandler* CastGLFW() override {return this; }

				ENGINE_API void Create(IWindow* a_window) override;
				ENGINE_API void Destroy() override;

				ENGINE_API bool IsKeyDown(Key a_key) override;
				ENGINE_API bool IsKeyPressed(Key a_key) override;
				ENGINE_API bool IsKeyReleased(Key a_key) override;

				ENGINE_API bool IsMouseButtonDown(MouseButton a_mouseButton) override;
				ENGINE_API bool IsMouseButtonPressed(MouseButton a_mouseButton) override;
				ENGINE_API bool IsMouseButtonReleased(MouseButton a_mouseButton) override;

				ENGINE_API Math::vec2 GetCursorPosition() override;


			private:
				static std::vector<int> m_currentKeyStatus;
				static std::vector<int> m_currentMouseButtonStatus;
				static Math::vec2 m_cursorPosition;

				static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
				static void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
				static void CursorPositionCallback(GLFWwindow* a_window, double a_xpos, double a_ypos);

			};
		}
	}
}

#endif