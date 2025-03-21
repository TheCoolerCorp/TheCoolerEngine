#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "EngineExport.h"
#include "ImGuiLayer.h"

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
			Application() = default;
			~Application() = default;

			void Create(int a_width, int a_height);
			void Run();
			void Destroy();
			Window::IWindow* GetWindow() const { return m_mainWindow; }

		private:
			Window::IWindow* m_mainWindow = nullptr;
			std::unique_ptr<ImGuiLayer> m_imGuiLayer;
			Renderer* m_renderer = nullptr;
			std::vector<GamePlay::GameObjectData> m_gameObjectDatas;
			std::vector<GamePlay::GameObject*> m_gameObjects;
		};
	}
}

#endif