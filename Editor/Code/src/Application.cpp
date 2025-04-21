#include "../Inlude/Application.h"

#include "Core/Window/GLInputHandler.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Components/MAterialComponent.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "GamePlay/Others/Scene.h"
#include "Math/TheCoolerMath.h"

#include "../Inlude/ImGuiLayer.h"

using namespace Engine::Core;
using namespace Engine::GamePlay;
using namespace Engine;
using namespace Engine::Math;
using namespace Editor::EditorLayer;

namespace Editor
{
	namespace Core
	{
		void Application::Create(const int a_width, const int a_height)
		{
			GraphicsAPI::VulkanRenderPassManager::AddFlag(GraphicsAPI::FLAG_VK_RHI_OVERRIDE_DEFAULT_RENDERPASS);

			m_mainWindow = new Window::GLwindow();
			m_mainWindow->Create(a_width, a_height);
			m_inputHandler = new Window::GLInputHandler();
			m_inputHandler->Create(m_mainWindow);

			m_renderer = new Renderer();
			m_renderer->Init(RendererType::VULKAN);
			m_renderer->Create(RendererType::VULKAN, m_mainWindow);

			Ui::ImGuiLayer* t_imguiLayer = new Ui::ImGuiLayer(m_renderer);
			AddLayer(t_imguiLayer);

			m_renderer->LateCreate(RendererType::VULKAN, m_mainWindow);

			m_camera = new Camera(vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f),
			                                        vec3(0.f, 1.f, -3.f), ToRadians(70.f),
			                                        static_cast<float>(a_width) / static_cast<float>(a_height), 0.1f, 100.f, 10.f, 2.f);
			m_camera->Create(m_renderer);

			m_currentScene = new Scene();
			m_currentScene->Create(m_renderer);
		}

		void Application::Run()
		{
			while (!m_mainWindow->ShouldClose())
			{
				auto t_now = std::chrono::high_resolution_clock::now();
				m_deltaTime = std::chrono::duration<float>(t_now - m_lastTime).count();
				m_lastTime = t_now;

				UpdateLayers();
				m_currentScene->Update(m_renderer, m_deltaTime);
				m_currentScene->Draw(m_renderer, m_mainWindow, m_camera);
				m_camera->Update(m_renderer, m_inputHandler, m_mainWindow, m_deltaTime);
				m_mainWindow->PollEvents();
			}
			m_renderer->WaitIdle();
		}

		void Application::Destroy()
		{
			CleanupLayers();

			m_camera->Destroy(m_renderer);
			delete m_camera;

			m_currentScene->Destroy(m_renderer);
			delete m_currentScene;

			m_renderer->Destroy();
			delete m_renderer;

			m_inputHandler->Destroy();
			delete m_inputHandler;

			m_mainWindow->Destroy();
			delete m_mainWindow;
		}

		void Application::AddLayer(Layer* a_layer)
		{
			a_layer->OnAttach(m_mainWindow);
			m_layers.push_back(a_layer);
		}

		void Application::UpdateLayers()
		{
			for (Layer* layer : m_layers)
			{
				layer->OnUpdate(m_deltaTime);
				//layer->OnUiRender();
			}
		}

		void Application::CleanupLayers()
		{
			for (Layer* layer : m_layers)
			{
				layer->Delete();
				delete layer;
			}
			m_layers.clear();
		}
	}
}
