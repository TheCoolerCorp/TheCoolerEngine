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

#include "Layer.h"

namespace Editor
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
			Engine::Core::Window::IWindow* GetWindow() const { return m_mainWindow; }

			void AddLayer(EditorLayer::Layer* a_layer);
			void UpdateLayers();
			void LayerProcessInput(Engine::Core::Window::IInputHandler* a_handler, float a_deltaTime);
			void CleanupLayers();

			void RegisterTypes();

			[[nodiscard]] Engine::GamePlay::Scene* GetCurrentScene() const { return m_currentScene; }
		private:
			Engine::Core::Window::IWindow* m_mainWindow = nullptr;
			Engine::Core::Window::IInputHandler* m_inputHandler = nullptr;
			Engine::Core::Renderer* m_renderer = nullptr;

			Engine::GamePlay::Camera* m_camera = nullptr;
			Engine::Resource::ResourceManager* m_resourceManager = nullptr;

			Engine::GamePlay::Scene* m_currentScene;
			std::vector<Engine::GamePlay::Scene*> m_scenes;
			std::chrono::high_resolution_clock::time_point m_lastTime = std::chrono::high_resolution_clock::now();
			float m_deltaTime = 0.0f;

			std::vector<EditorLayer::Layer*> m_layers;

			void UpdateDeltaTime();
		};
	}
}

#endif