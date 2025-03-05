#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "EngineExport.h"

#include "Core/Window/IWindow.h"

#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"


namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			class ENGINE_API GLwindow : public IWindow
			{
			public:
				~GLwindow() override = default;
				GLwindow* CastGLFW() override { return nullptr; }

				void Create(int a_width, int a_height) override;
				void Destroy() override;

				void PollEvents() override;
				void WaitEvents() override;
				bool ShouldClose() override;
				void GetFramebufferSize(int& a_width, int& a_height) override;

				const char** GetRequiredInstanceExtensions(uint32_t* count) override;

				bool GetResized() override { return m_resized; }
			private:
				GLFWwindow* m_window = nullptr;
				int m_width = 800;
				int m_height = 600;
				bool m_resized = false;

				static void ResizeFramebuffer(GLFWwindow* a_window, int a_width, int a_height);
			};
		}
	}
}
#endif