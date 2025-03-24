#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "EngineExport.h"
#include "ImGuiLayer.h"

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"

#include "Core/Window/GLWindow.h"
#include "Core/Renderer/Renderer.h"
#include "GamePlay/GameObject.h"
#include "Layer.h"

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

			void InitLayers();
			void UpdateLayers();
			void DestroyLayers();

			Window::IWindow* GetWindow() const { return m_mainWindow; }

		private:
			Window::IWindow* m_mainWindow = nullptr;
			Renderer* m_renderer = nullptr;
			std::vector<GamePlay::GameObjectData> m_gameObjectDatas;
			std::vector<GamePlay::GameObject*> m_gameObjects;

			std::vector<Editor::Core::Layer*> m_layers;
		};
	}
}

#endif