#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>


#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"
#include "ImguiCooler.h"
#include "Core/Window/GLWindow.h"
#include "Core/Window/GLInputHandler.h"
#include "Core/Renderer/Renderer.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Others/Camera.h"
#include "GamePlay/Others/Scene.h"

namespace Engine
{
	namespace Core
	{
		class Application
		{
		public:
			Application() = default;
			~Application() = default;

			void Create(int a_width, int a_height);
			void Run();
			void Destroy();
			Window::IWindow* GetWindow() const { return m_mainWindow; }

		private:
			Window::IWindow* m_mainWindow = nullptr;
			Window::IInputHandler* m_inputHandler = nullptr;
			Renderer* m_renderer = nullptr;

			GamePlay::Camera* m_camera = nullptr;

			GamePlay::Scene* m_currentScene;
			std::vector<GamePlay::Scene*> m_scenes;
			std::chrono::high_resolution_clock::time_point m_lastTime = std::chrono::high_resolution_clock::now();
			float m_deltaTime = 0.0f;
			UI ui;
		};
	}
}

#endif