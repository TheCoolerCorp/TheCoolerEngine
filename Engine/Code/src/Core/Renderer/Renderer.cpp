#include "Core/Renderer/Renderer.h"
#include "Core/Logger/Logger.h"

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
		}

		void Renderer::Run()
		{

		}

		void Renderer::Destroy()
		{
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