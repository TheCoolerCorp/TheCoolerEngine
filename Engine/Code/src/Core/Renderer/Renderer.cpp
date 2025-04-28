#include "Core/Renderer/Renderer.h"
#include "Core/Logger/Logger.h"
#include "GamePlay/Others/GameObject.h"
#include "Ressources/Texture.h"
#include "Ressources/Mesh.h"
namespace Engine
{
	namespace Core
	{
		void Renderer::Init(RendererType a_type)
		{
			if (a_type != RendererType::VULKAN)
			{
				LOG_ERROR("Only vulkan is implemented !");
			}
			m_interface = new GraphicsAPI::VulkanInterface();
			m_apiInstance = m_interface->InstantiateInstance();
			m_valiationLayers = m_interface->InstantiateValidationLayers();
			m_surface = m_interface->InstantiateSurface();
			m_physicalDevice = m_interface->InstantiatePhysicalDevice();
			m_logicalDevice = m_interface->InstantiateLogicalDevice();
			m_swapChain = m_interface->InstantiateSwapChain();
			m_renderPass = m_interface->InstantiateRenderPass();
			m_unlitPipeline = m_interface->InstantiateGraphicPipeline();
			m_litPipeline = m_interface->InstantiateGraphicPipeline();
			m_commandPool = m_interface->InstantiateCommandPool();
		}

		void Renderer::Create(RendererType a_type, Window::IWindow* a_window)
		{
			if (a_type != RendererType::VULKAN)
			{
				LOG_ERROR("Only vulkan is implemented !");
			}
			
			m_apiInstance->Create();
			
			m_valiationLayers->Create(m_apiInstance);
			
			m_surface->Create(a_window, m_apiInstance);
			
			m_physicalDevice->Create(m_apiInstance, m_surface);
			
			m_logicalDevice->Create(m_physicalDevice, m_surface);

			m_surface->SetupInfo(m_physicalDevice);
			
			m_swapChain->Create(m_surface, a_window, m_physicalDevice, m_logicalDevice);
			m_renderPass->Create(this);
			m_renderPass->CastVulkan()->CreateDefaultRenderPass(this);

			


			
			m_swapChain->CreateSyncObjects(m_logicalDevice);
		}

		void Renderer::LateCreate(RendererType a_type, Window::IWindow* a_window)
		{
			RHI::IShader* t_vertexShader = m_interface->InstantiateShader();
			RHI::IShader* t_unlitFragmentShader = m_interface->InstantiateShader();
			RHI::IShader* t_litFragmentShader = m_interface->InstantiateShader();
			t_vertexShader->Create(BASEVERTEX, m_logicalDevice);
			t_unlitFragmentShader->Create(UNLITFRAGMENT, m_logicalDevice);
			t_litFragmentShader->Create(LITFRAGMENT, m_logicalDevice);
			std::array<RHI::IShader*, 2> t_unlitVertAndFrag = { t_vertexShader, t_unlitFragmentShader };
			std::array<RHI::IShader*, 2> t_litVertAndFrag = { t_vertexShader, t_litFragmentShader };

			m_unlitPipeline->Create(m_logicalDevice, m_renderPass, RHI::Unlit, t_unlitVertAndFrag);
			m_litPipeline->Create(m_logicalDevice, m_renderPass, RHI::Lit, t_litVertAndFrag);

			t_vertexShader->Destroy(m_logicalDevice);
			t_unlitFragmentShader->Destroy(m_logicalDevice);
			t_litFragmentShader->Destroy(m_logicalDevice);
			m_interface->DestroyShader(t_vertexShader);
			m_interface->DestroyShader(t_unlitFragmentShader);
			m_interface->DestroyShader(t_litFragmentShader);

			m_commandPool = m_interface->InstantiateCommandPool();
			m_commandPool->Create(m_physicalDevice, m_surface, m_logicalDevice);
			m_swapChain->CreateFramebuffers(m_logicalDevice, m_physicalDevice, m_renderPass, m_commandPool);
			m_commandPool->CreateCommandBuffer(m_logicalDevice, m_swapChain->GetMaxFrame());
		}
		void Renderer::WaitIdle() const
		{
			m_logicalDevice->WaitIdle();
		}

		void Renderer::Destroy()
		{

			m_commandPool->Destroy(m_logicalDevice);
			m_interface->DestroyCommandPool(m_commandPool);

			m_unlitPipeline->Destroy(m_logicalDevice);
			m_interface->DestroyGraphicPipeline(m_unlitPipeline);
			m_litPipeline->Destroy(m_logicalDevice);
			m_interface->DestroyGraphicPipeline(m_litPipeline);

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