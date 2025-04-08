#include "Core/Window/GLWindow.h"
#include "Core/Logger/Logger.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			void GLwindow::Create(const int a_width, const int a_height)
			{
				m_width = a_width;
				m_height = a_height;

				glfwInit();

				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

				m_window = glfwCreateWindow(m_width, m_height, "TheCoolerEngine", nullptr, nullptr);
				glfwSetWindowUserPointer(m_window, this);

				glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCallback);
			}

			void GLwindow::Destroy()
			{
				glfwDestroyWindow(m_window);
				glfwTerminate();
			}


			void GLwindow::PollEvents()
			{
				glfwPollEvents();
			}

			void GLwindow::WaitEvents()
			{
				glfwWaitEvents();
			}

			bool GLwindow::ShouldClose()
			{
				bool shouldClose = glfwWindowShouldClose(m_window);
				return shouldClose;
			}

			void GLwindow::FramebufferResizeCallback(GLFWwindow* a_window, const int a_width, const int a_height)
			{
				const auto t_app = reinterpret_cast<GLwindow*>(glfwGetWindowUserPointer(a_window));

				t_app->m_resized = true;
				t_app->m_width = a_width;
				t_app->m_height = a_height;

				glfwSetWindowSize(t_app->m_window, t_app->m_width, t_app->m_height);
			}

			const char** GLwindow::GetRequiredInstanceExtensions(uint32_t* a_count)
			{
				return glfwGetRequiredInstanceExtensions(a_count);
			}

			void GLwindow::ResizeFramebuffer()
			{
				int t_width = 0, t_height = 0;
				glfwGetFramebufferSize(m_window, &t_width, &t_height);
				while (t_width == 0 || t_height == 0) 
				{
					glfwGetFramebufferSize(m_window, &t_width, &t_height);
					glfwWaitEvents();
				}
			}

			void GLwindow::CaptureCursor(const bool a_capture)
			{
				int t_value = 0;
				if (a_capture)
				{
					t_value = GLFW_CURSOR_DISABLED;
				}
				else
				{
					t_value = GLFW_CURSOR_NORMAL;
				}
				glfwSetInputMode(m_window, GLFW_CURSOR, t_value);
			}

			void GLwindow::GetFramebufferSize(int* a_width, int* a_height)
			{
				glfwGetFramebufferSize(m_window, a_width, a_height);
			}

			void GLwindow::CreateWindowSurface(RHI::IInstance* a_instance, RHI::ISurface* a_surface)
			{
				VkSurfaceKHR& t_surface = a_surface->CastVulkan()->GetVkSurfaceKHR();
				VK_CHECK(glfwCreateWindowSurface(a_instance->CastVulkan()->GetVkInstance(), m_window, nullptr, &t_surface), "failed to create window surface!");
			}
		}
	}
}