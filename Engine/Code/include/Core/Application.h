#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "EngineExport.h"

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"

#include "Core/Window/GLWindow.h"
#include "Core/Renderer/Renderer.h"

namespace Engine
{
	namespace GamePlay
	{
		struct GameObjectData;
	}

	namespace Core
	{
		class Application
		{
		public:
			ENGINE_API Application() = default;
			ENGINE_API ~Application() = default;

			ENGINE_API void Create(int a_width, int a_height);
			ENGINE_API void Run() const;
			ENGINE_API void Destroy() const;
			ENGINE_API Window::IWindow* GetWindow() const { return m_mainWindow; }

		private:
			Window::IWindow* m_mainWindow = nullptr;
			Renderer* m_renderer = nullptr;
			std::vector<GamePlay::GameObjectData> m_gameObjectDatas{};
		};
	}
}

#endif