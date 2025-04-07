#include "Core/Renderer/Renderer.h"
#include "Core/Logger/Logger.h"
#include "GamePlay/Others/GameObject.h"
#include "Ressources/Texture.h"
#include "Ressources/Mesh.h"
namespace Engine
{
	namespace Core
	{
		void Renderer::Init(RendererType a_type, Window::IWindow* a_window)
		{
			if (a_type != RendererType::VULKAN)
			{
				LOG_ERROR("Only vulkan is implemented !");
			}
			m_interface = new GraphicsAPI::VulkanInterface();

			m_apiInstance = m_interface->InstantiateInstance();
			m_apiInstance->Create();

			m_valiationLayers = m_interface->InstantiateValidationLayers();
			m_valiationLayers->Create(m_apiInstance);

			m_surface = m_interface->InstantiateSurface();
			m_surface->Create(a_window, m_apiInstance);

			m_physicalDevice = m_interface->InstantiatePhysicalDevice();
			m_physicalDevice->Create(m_apiInstance, m_surface);

			m_logicalDevice = m_interface->InstantiateLogicalDevice();
			m_logicalDevice->Create(m_physicalDevice, m_surface);

			m_surface->SetupInfo(m_physicalDevice);

			m_swapChain = m_interface->InstantiateSwapChain();
			m_swapChain->Create(m_surface, a_window, m_physicalDevice, m_logicalDevice);

			m_renderPass = m_interface->InstantiateRenderPass();
			m_renderPass->Create(m_swapChain, m_physicalDevice, m_logicalDevice);

			m_graphicPipeline = m_interface->InstantiateGraphicPipeline();
			m_graphicPipeline->Create(m_logicalDevice, m_renderPass);

			m_commandPool = m_interface->InstantiateCommandPool();
			m_commandPool->Create(m_physicalDevice, m_surface, m_logicalDevice);
			m_swapChain->CreateFramebuffers(m_logicalDevice, m_physicalDevice, m_renderPass, m_commandPool);


			m_commandPool->CreateCommandBuffer(m_logicalDevice, m_swapChain, m_renderPass, m_graphicPipeline);

			m_swapChain->CreateSyncObjects(m_logicalDevice);
		}

		void Renderer::Render(Window::IWindow* a_window,
		                      const std::unordered_map<int, Core::RHI::IRenderObject*>& a_renderObjects,
		                      const std::vector<int>& a_ids, const std::unordered_map<int, Core::RHI::IBuffer*>& a_vertexBuffers,
		                      const std::unordered_map<int, Core::RHI::IBuffer*>& a_indexBuffers, const std::unordered_map<int, uint32_t>& a_nbIndices,
		                      GamePlay::Camera* camera) const
		{
			//m_swapChain->DrawFrame(a_window, m_logicalDevice, m_commandPool, m_surface, m_physicalDevice, m_renderPass, a_renderObjects, a_ids, a_vertexBuffers, a_indexBuffers, a_nbIndices, camera);
		}

		void Renderer::WaitIdle() const
		{
			m_logicalDevice->WaitIdle();
		}

		void Renderer::Destroy()
		{

			m_commandPool->Destroy(m_logicalDevice);
			m_interface->DestroyCommandPool(m_commandPool);

			m_graphicPipeline->Destroy(m_logicalDevice);
			m_interface->DestroyGraphicPipeline(m_graphicPipeline);

			m_renderPass->Destroy(m_logicalDevice);
			m_interface->DestroyRenderPass(m_renderPass);

			m_swapChain->Destroy(m_logicalDevice);
			m_interface->DestroySwapChain(m_swapChain);

			m_logicalDevice->Destroy();
			m_interface->DestroyLogicalDevice(m_logicalDevice);

			m_physicalDevice->Destroy();
			m_interface->DestroyPhysicalDevice(m_physicalDevice);

			m_surface->Destroy(m_apiInstance);
			m_interface->DestroySurface(m_surface);

			m_valiationLayers->Destroy(m_apiInstance);
			m_interface->DestroyValidationLayers(m_valiationLayers);

			m_apiInstance->Destroy();
			m_interface->DestroyInstance(m_apiInstance);

			delete m_interface;
		}
	}
}