#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "EngineExport.h"

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"

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
			ENGINE_API Application() = default;
			ENGINE_API ~Application() = default;

			ENGINE_API void Create(int a_width, int a_height);
			ENGINE_API void Run();
			ENGINE_API void Destroy();
			ENGINE_API Window::IWindow* GetWindow() const { return m_mainWindow; }

		private:
			void UpdateDeltaTime();

			Window::IWindow* m_mainWindow = nullptr;
			Window::IInputHandler* m_inputHandler = nullptr;
			Renderer* m_renderer = nullptr;

			GamePlay::Scene* m_currentScene;
			std::vector<GamePlay::Scene*> m_scenes;
			std::chrono::high_resolution_clock::time_point m_lastTime = std::chrono::high_resolution_clock::now();
			float m_deltaTime = 0.0f;
		};
	}
}

#endif