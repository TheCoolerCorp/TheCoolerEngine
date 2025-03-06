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
			void GLwindow::Create(int a_width, int a_height)
			{
				m_width = a_width;
				m_height = a_height;

				glfwInit();

				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

				m_window = glfwCreateWindow(m_width, m_height, "TheCoolerEngine", nullptr, nullptr);
				glfwSetWindowUserPointer(m_window, this);

				glfwSetFramebufferSizeCallback(m_window, ResizeFramebuffer);
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

			void GLwindow::ResizeFramebuffer(GLFWwindow* a_window, int a_width, int a_height)
			{
				auto app = reinterpret_cast<GLwindow*>(glfwGetWindowUserPointer(a_window));

				app->m_resized = true;
				app->m_width = a_width;
				app->m_height = a_height;

				glfwSetWindowSize(app->m_window, app->m_width, app->m_height);
			}

			const char** GLwindow::GetRequiredInstanceExtensions(uint32_t* count)
			{
				return glfwGetRequiredInstanceExtensions(count);
			}

			void GLwindow::GetFramebufferSize(int& a_width, int& a_height)
			{
				a_width = m_width;
				a_height = m_height;
			}

			void GLwindow::CreateWindowSurface(RHI::IInstance* a_instance, RHI::ISurface* a_surface)
			{
				VkSurfaceKHR& t_surface = a_surface->CastVulkan()->GetVkSurfaceKHR();
				VK_CHECK(glfwCreateWindowSurface(a_instance->CastVulkan()->GetVkInstance(), m_window, nullptr, &t_surface), "failed to create window surface!");
			}
		}
	}
}