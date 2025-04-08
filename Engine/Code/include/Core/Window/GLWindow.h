#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "EngineExport.h"

#include "Core/Window/IWindow.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"


namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			class GLwindow : public IWindow
			{
			public:
				ENGINE_API ~GLwindow() override = default;
				ENGINE_API GLwindow* CastGLFW() override { return this; }

				ENGINE_API void Create(int a_width, int a_height) override;
				ENGINE_API void Destroy() override;

				ENGINE_API void PollEvents() override;
				ENGINE_API void WaitEvents() override;
				ENGINE_API bool ShouldClose() override;
				ENGINE_API void GetFramebufferSize(int* a_width, int* a_height) override;
				ENGINE_API void CreateWindowSurface(RHI::IInstance* a_instance, RHI::ISurface* a_surface) override;

				ENGINE_API const char** GetRequiredInstanceExtensions(uint32_t* a_count) override;

				ENGINE_API bool GetResized() override { return m_resized; }
				ENGINE_API void SetResized(const bool a_resized) override { m_resized = a_resized; }
				ENGINE_API GLFWwindow* GetWindow() { return m_window; }

				ENGINE_API void ResizeFramebuffer() override;

				ENGINE_API void CaptureCursor(bool a_capture) override;
			private:
				GLFWwindow* m_window = nullptr;
				int m_width = 800;
				int m_height = 600;
				bool m_resized = false;

				ENGINE_API static void FramebufferResizeCallback(GLFWwindow* a_window, int a_width, int a_height);
			};
		}
	}
}
#endif