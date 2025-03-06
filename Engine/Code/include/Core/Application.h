#ifndef APPLICATION_H
#define APPLICATION_H

#include "EngineExport.h"

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"

#include "Core/Window/GLWindow.h"
#include "Core/Renderer/Renderer.h"

namespace Engine
{
	namespace Core
	{
		class ENGINE_API Application
		{
		public:
			Application() = default;
			~Application() = default;

			void Create(int a_width, int a_height);
			void Run() const;
			void Destroy() const;
			Window::IWindow* GetWindow() const { return m_mainWindow; }

		private:
			Window::IWindow* m_mainWindow = nullptr;
			Renderer* m_renderer = nullptr;
		};
	}
}

#endif