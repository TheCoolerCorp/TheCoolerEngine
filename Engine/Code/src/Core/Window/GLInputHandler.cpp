#include "Core/Window/GLInputHandler.h"

namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			std::vector<int>GLInputHandler::m_currentKeyStatus(400, 0);
			std::vector<int>GLInputHandler::m_currentMouseButtonStatus(50, 0);


			void GLInputHandler::Create(IWindow* a_window)
			{
				GLFWwindow* t_window = a_window->CastGLFW()->GetWindow();

				glfwSetKeyCallback(t_window, KeyCallBack);
				glfwSetMouseButtonCallback(t_window, MouseCallBack);
			}

			void GLInputHandler::Destroy()
			{
				m_currentKeyStatus.clear();
				m_currentMouseButtonStatus.clear();
			}

			bool GLInputHandler::IsKeyDown(Key a_key)
			{
				return (m_currentKeyStatus[static_cast<int>(a_key)] == GLFW_PRESS) ||
					(m_currentKeyStatus[static_cast<int>(a_key)] == GLFW_REPEAT);
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

			void GLInputHandler::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				m_currentKeyStatus[key] = action;
			}

			void GLInputHandler::MouseCallBack(GLFWwindow* window, int button, int action, int mods)
			{
				m_currentMouseButtonStatus[button] = action;
			}
		}
	}
}
