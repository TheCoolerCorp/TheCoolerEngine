#include "Application.h"

#include "FileExplorerWindow.h"
#include "Core/Window/GLInputHandler.h"
#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "GamePlay/Others/Scene.h"
#include "Math/TheCoolerMath.h"
#include "SceneGraphWindow.h"
#include "ImGuiLayer.h"
#include "InspectorWindow.h"
#include "LoggerWindow.h"
#include "Core/Audio/SoundManager.h"

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
			RegisterTypes();

			Audio::SoundManager::Get().Init();

			GraphicsAPI::VulkanRenderPassManager::AddFlag(GraphicsAPI::FLAG_VK_RHI_OVERRIDE_DEFAULT_RENDERPASS);

			m_mainWindow = new Window::GLwindow();
			m_mainWindow->Create(a_width, a_height);
			m_inputHandler = new Window::GLInputHandler();
			m_inputHandler->Create(m_mainWindow);

			m_renderer = new Renderer();
			m_renderer->Init(RendererType::VULKAN);
			m_renderer->Create(RendererType::VULKAN, m_mainWindow);

			Ui::ImGuiLayer* t_imguiLayer = new Ui::ImGuiLayer(m_renderer, this);
			
			AddLayer(t_imguiLayer);

			m_renderer->LateCreate(RendererType::VULKAN, m_mainWindow);

			m_resourceManager = new Resource::ResourceManager;
			ServiceLocator::ProvideResourceManager(m_resourceManager);

			Ref<Resource::Mesh> t_capsuleMesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/WireframeCapsule.obj");
			Ref<Resource::Mesh> t_cubeMesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/WireframeCube.obj");
			Ref<Resource::Mesh> t_sphereMesh = m_resourceManager->CreateResource<Resource::Mesh>("Assets/Meshes/WireframeSphere.obj");
			Ref<Resource::Texture> t_colliderTexture = m_resourceManager->CreateResource<Resource::Texture>("Assets/Textures/ColliderTexture.png");
			t_capsuleMesh->Load();
			t_capsuleMesh->BindBuffers(m_renderer);
			t_cubeMesh->Load();
			t_cubeMesh->BindBuffers(m_renderer);
			t_sphereMesh->Load();
			t_sphereMesh->BindBuffers(m_renderer);
			t_colliderTexture->Load();
			t_colliderTexture->CreateImage(m_renderer);

			m_currentScene = new Scene();
			m_currentScene->Create(m_renderer, "MainScene", a_width, a_height);

			t_imguiLayer->AddWindow(new Ui::SceneGraphUiWindow(m_renderer, t_imguiLayer, m_currentScene));
			t_imguiLayer->AddWindow(new Ui::InspectorUiWindow(m_renderer, t_imguiLayer));
			t_imguiLayer->AddWindow(new Ui::UiLoggerWindow(m_renderer, t_imguiLayer));
			t_imguiLayer->AddWindow(new Ui::FileExplorerWindow(m_renderer, t_imguiLayer));

			Audio::SoundManager::Get().PlaySound("Assets/Sounds/oxp.wav");
		}

		void Application::Run()
		{
			while (!m_mainWindow->ShouldClose())
			{
				UpdateDeltaTime();
				m_currentScene->Update(m_renderer, m_mainWindow, m_inputHandler, m_deltaTime);
				UpdateLayers();
				LayerProcessInput(m_inputHandler, m_deltaTime);

				/*
				 * Basic begin frame to acquired the next image index in the swapchain to draw in for presentation on screen
				 */
				uint32_t t_imageIndex = 0;
				m_renderer->GetSwapChain()->BeginFrame(m_renderer->GetLogicalDevice(), &t_imageIndex);

				/*
				 * Choose the set of commandBuffers to begin and the index of this set of commandBuffers to begin
				 */
				m_renderer->GetCommandPool()->BeginCommand(0, m_renderer->GetSwapChain()->GetCurrentFrame());

				GraphicsAPI::RecordRenderPassinfo t_info;
				t_info.renderer = m_renderer;
				t_info.scene = m_currentScene;
				t_info.commandPoolIndex = 0;
				t_info.imageIndex = t_imageIndex;
				t_info.currentFrame = m_renderer->GetSwapChain()->GetCurrentFrame();

				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>> t_vertexBuffers = m_currentScene->GetVertexBuffers();
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>> t_indexBuffers = m_currentScene->GetIndexBuffers();
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>> t_nbIndices = m_currentScene->GetNBIndices();
				std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>> t_descriptors = m_currentScene->GetDescriptors();

				m_renderer->GetRenderPass()->CastVulkan()->RecordRenderPasses(t_info, t_vertexBuffers, t_indexBuffers, t_nbIndices, t_descriptors);

				/*
				 * Choose the set of commandBuffers to end and the index of this set of commandBuffers to end
				 */
				m_renderer->GetCommandPool()->EndCommand(0, m_renderer->GetSwapChain()->GetCurrentFrame());


				/*
				 * Basic end frame to submit data and present image
				 */
				m_renderer->GetSwapChain()->EndFrame(m_renderer->GetLogicalDevice(), m_renderer->GetCommandPool(), m_renderer->GetSurface(), m_renderer->GetPhysicalDevice(), m_renderer->GetRenderPass(), m_mainWindow, t_imageIndex);

				m_mainWindow->PollEvents();
			}
			m_renderer->WaitIdle();
		}

		void Application::Destroy()
		{
			CleanupLayers();

			m_currentScene->Destroy(m_renderer);
			delete m_currentScene;

			m_resourceManager->DestroyAll(m_renderer);
			delete m_resourceManager;

			m_renderer->Destroy();
			delete m_renderer;

			m_inputHandler->Destroy();
			delete m_inputHandler;

			m_mainWindow->Destroy();
			delete m_mainWindow;

			Audio::SoundManager::Get().UnInit();
		}

		void Application::UpdateDeltaTime()
		{
			auto t_now = std::chrono::high_resolution_clock::now();
			m_deltaTime = std::chrono::duration<float>(t_now - m_lastTime).count();
			m_lastTime = t_now;
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

		void Application::LayerProcessInput(Engine::Core::Window::IInputHandler* a_handler, float a_deltaTime)
		{
			for (Layer* layer : m_layers)
			{
				layer->OnProcessInputs(a_handler, a_deltaTime);
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

		void Application::RegisterTypes()
		{
			Engine::GamePlay::TransformComponent::Register();
			Engine::GamePlay::RigidBodyComponent::Register();
			Engine::GamePlay::MeshComponent::Register();
			Engine::GamePlay::LightComponent::Register();
			Engine::GamePlay::CameraComponent::Register();
		}
	}
}
