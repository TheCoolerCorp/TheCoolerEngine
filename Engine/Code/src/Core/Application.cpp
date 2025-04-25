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
				m_renderer->GetCommandPool()->BeginCommand(0, m_renderer->GetSwapChain()->GetCurrentFrame());


				/*
				 * Begin the renderpass
				 */
				m_renderer->GetRenderPass()->BeginRenderPass(m_renderer->GetCommandPool(), 0,  m_renderer->GetSwapChain(), t_imageIndex);


				std::vector<RHI::IBuffer*> t_vertexBuffers = m_currentScene->GetVertexBuffers();
				std::vector<RHI::IBuffer*> t_indexBuffers = m_currentScene->GetVertexBuffers();
				std::vector<uint32_t> t_nbIndices = m_currentScene->GetNBIndices();
				std::vector<RHI::IObjectDescriptor*> t_descriptors = m_currentScene->GetDescriptors();

				std::vector<RHI::IBuffer*> t_unlitVertexBuffers;
				std::vector<RHI::IBuffer*> t_unlitIndexBuffers;
				std::vector<uint32_t> t_unlitNbIndices;
				std::vector<RHI::IObjectDescriptor*> t_unlitDescriptors;

				std::vector<RHI::IBuffer*> t_litVertexBuffers;
				std::vector<RHI::IBuffer*> t_litIndexBuffers;
				std::vector<uint32_t> t_litnbIndices;
				std::vector<RHI::IObjectDescriptor*> t_litdescriptors;

				for (int i = 0; i < t_descriptors.size(); ++i)
				{
					if (t_descriptors[i]->GetPipelineTargetType() == RHI::LitDescriptor)
					{
						t_unlitDescriptors.push_back(t_descriptors[i]);
						t_unlitVertexBuffers.push_back(t_vertexBuffers[i]);
						t_unlitIndexBuffers.push_back(t_indexBuffers[i]);
						t_unlitNbIndices.push_back(t_nbIndices[i]);
					}
					else
					{
						t_litdescriptors.push_back(t_descriptors[i]);
						t_litVertexBuffers.push_back(t_vertexBuffers[i]);
						t_litIndexBuffers.push_back(t_indexBuffers[i]);
						t_litnbIndices.push_back(t_nbIndices[i]);
					}
				}
				t_descriptors.clear();
#pragma region Unlit
				/*
				 * Bind the pipeline to use
				 */
				m_renderer->GetUnlitPipeline()->Bind(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain());


				/*
				* Bind decriptors only (for special case like lights, cubemap, camera, etc...)
				*/
				m_renderer->GetUnlitPipeline()->BindSingleDescriptors(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain()->GetCurrentFrame(),
												t_imageIndex, {m_currentScene->GetCameraDescriptor()});


				/*
				* Bind decriptors and sent buffer
				*/
				m_renderer->GetUnlitPipeline()->BindObjects(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain()->GetCurrentFrame(),
												t_imageIndex, t_unlitIndexBuffers, t_unlitVertexBuffers, t_unlitNbIndices, t_unlitDescriptors);
				t_unlitIndexBuffers.clear();
				t_unlitVertexBuffers.clear();
				t_unlitNbIndices.clear();
				t_unlitDescriptors.clear();
#pragma endregion

#pragma  region Lit
				/*
				 * Bind the pipeline to use
				 */
				//m_renderer->GetLitPipeline()->Bind(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain());


				/*
				* Bind decriptors only (for special case like lights, cubemap, camera, etc...)
				*/
				//m_renderer->GetLitPipeline()->BindSingleDescriptors(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain()->GetCurrentFrame(),
					//t_imageIndex, { m_currentScene->GetCameraDescriptor() });


				/*
				* Bind decriptors and sent buffer
				*/
				//m_renderer->GetUnlitPipeline()->BindObjects(m_renderer->GetCommandPool(), 0, m_renderer->GetSwapChain()->GetCurrentFrame(),
					//t_imageIndex, t_litIndexBuffers, t_litVertexBuffers, t_litnbIndices, t_litdescriptors);

				t_litIndexBuffers.clear();
				t_litVertexBuffers.clear();
				t_litnbIndices.clear();
				t_litdescriptors.clear();
#pragma endregion
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
