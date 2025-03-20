#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "EngineExport.h"

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"

#include "Core/Window/GLWindow.h"
#include "Core/Renderer/Renderer.h"
#include "GamePlay/GameObject.h"

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
			Window::IWindow* m_mainWindow = nullptr;
			Renderer* m_renderer = nullptr;
			std::vector<GamePlay::GameObjectData> m_gameObjectDatas;
			std::vector<GamePlay::GameObject*> m_gameObjects;
		};
	}
}

#endif