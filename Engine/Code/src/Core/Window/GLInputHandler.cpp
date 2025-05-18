#include "Core/Window/GLInputHandler.h"

namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			std::vector<int>GLInputHandler::m_currentKeyStatus(349, 0);
			std::vector<int>GLInputHandler::m_currentMouseButtonStatus(12, 0);
			Math::vec2 GLInputHandler::m_cursorPosition = Math::vec2();

			void GLInputHandler::Create(IWindow* a_window)
			{
				GLFWwindow* t_window = a_window->CastGLFW()->GetWindow();

				glfwSetKeyCallback(t_window, KeyCallBack);
				glfwSetMouseButtonCallback(t_window, MouseCallBack);
				glfwSetCursorPosCallback(t_window, CursorPositionCallback);
			}

			void GLInputHandler::Destroy()
			{
				m_currentKeyStatus.clear();
				m_currentMouseButtonStatus.clear();
			}

			bool GLInputHandler::IsKeyDown(Key a_key)
			{
				return (m_currentKeyStatus[static_cast<int>(a_key)] == GLFW_PRESS);
			}

			bool GLInputHandler::IsKeyPressed(Key a_key)
			{
				return (m_currentKeyStatus[static_cast<int>(a_key)] == GLFW_REPEAT);
			}

			bool GLInputHandler::IsKeyReleased(Key a_key)
			{
				return (m_currentKeyStatus[static_cast<int>(a_key)] == GLFW_RELEASE);
			}


			bool GLInputHandler::IsMouseButtonDown(MouseButton a_mouseButton)
			{
				return (m_currentMouseButtonStatus[static_cast<int>(a_mouseButton)] == GLFW_PRESS) ||
					(m_currentMouseButtonStatus[static_cast<int>(a_mouseButton)] == GLFW_REPEAT);
			}

			bool GLInputHandler::IsMouseButtonPressed(MouseButton a_mouseButton)
			{
				return (m_currentMouseButtonStatus[static_cast<int>(a_mouseButton)] == GLFW_REPEAT);

			}

			bool GLInputHandler::IsMouseButtonReleased(MouseButton a_mouseButton)
			{
				return (m_currentMouseButtonStatus[static_cast<int>(a_mouseButton)] == GLFW_RELEASE);
			}

			Math::vec2 GLInputHandler::GetCursorPosition()
			{
				return m_cursorPosition;
			}

			void GLInputHandler::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				if (key == -1)
					return;
				m_currentKeyStatus[key] = action;
			}

			void GLInputHandler::MouseCallBack(GLFWwindow* window, int button, int action, int mods)
			{
				if (button == -1)
					return;
				m_currentMouseButtonStatus[button] = action;
			}

			void GLInputHandler::CursorPositionCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
			{
				m_cursorPosition.x = static_cast<float>(a_xpos);
				m_cursorPosition.y = static_cast<float>(a_ypos);
			}
		}
	}
}
