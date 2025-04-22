#include "Core/Application.h"

#include "GamePlay/Others/GameObject.h"
#include "GamePlay/Components/MAterialComponent.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
	namespace Core
	{
		void Application::Create(const int a_width, const int a_height)
		{
			m_mainWindow = new Window::GLwindow();
			m_mainWindow->Create(a_width, a_height);
			m_inputHandler = new Window::GLInputHandler();
			m_inputHandler->Create(m_mainWindow);

			m_renderer = new Renderer();
			m_renderer->Init(RendererType::VULKAN, m_mainWindow);

			m_currentScene = new GamePlay::Scene();
			m_currentScene->Create(m_renderer, a_width, a_height);
		}

		void Application::Run()
		{
			while (!m_mainWindow->ShouldClose())
			{
				UpdateDeltaTime();
				m_currentScene->Update(m_renderer, m_mainWindow, m_inputHandler, m_deltaTime);

				/*
				 * Basic begin frame to acquired the next image index in the swapchain to draw in for presentation on screen
				 */
				uint32_t t_imageIndex = 0;
				m_renderer->GetSwapChain()->BeginFrame(m_renderer->GetLogicalDevice(), &t_imageIndex);

				/*
				 * Choose the set of commandBuffers to begin and the index of this set of commandBuffers to begin
				 */
				m_renderer->GetCommandPool()->BeginCommand(0,m_renderer->GetSwapChain()->GetCurrentFrame());


				/*
				 * Begin the renderpass
				 */
				m_renderer->GetRenderPass()->BeginRenderPass(m_renderer->GetCommandPool(), 0,  m_renderer->GetSwapChain(), t_imageIndex);


				/*
				 * Bind the pipeline to use
				 */
				m_renderer->GetPipeline()->Bind(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain());


				/*
				* Bind decriptors only (for special case like lights, cubemap, camera, etc...)
				*/
				m_renderer->GetPipeline()->BindSingleDescriptors(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain()->GetCurrentFrame(),
												t_imageIndex, {m_currentScene->GetCameraDescriptor()});


				/*
				* Bind decriptors and sent buffer
				*/
				m_renderer->GetPipeline()->BindObjects(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain()->GetCurrentFrame(), 
												t_imageIndex, m_currentScene->GetIndexBuffers(), m_currentScene->GetVertexBuffers(), m_currentScene->GetNBIndices(), m_currentScene->GetDescriptors());

				/*
				 * End the renderpass 
				 */
				m_renderer->GetRenderPass()->EndRenderPass(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain()->GetCurrentFrame());


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
			m_currentScene->Destroy(m_renderer);
			delete m_currentScene;

			m_renderer->Destroy();
			delete m_renderer;

			m_inputHandler->Destroy();
			delete m_inputHandler;

			m_mainWindow->Destroy();
			delete m_mainWindow;
		}

		void Application::UpdateDeltaTime()
		{
			auto t_now = std::chrono::high_resolution_clock::now();
			m_deltaTime = std::chrono::duration<float>(t_now - m_lastTime).count();
			m_lastTime = t_now;
		}
	}
}
