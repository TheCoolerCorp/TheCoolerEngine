#include "Core/Renderer/Renderer.h"

namespace Engine
{
	namespace Core
	{
		void Renderer::Init(RendererType a_type)
		{
			if (a_type != RendererType::VULKAN)
			{
				// LOG ERROR NOT IMPLEMENTED
			}
			m_interface = new GraphicsAPI::VulkanInterface();


			m_apiInstance = m_interface->InstantiateInstance();
			m_apiInstance->Create();

			m_valiationLayers = m_interface->InstantiateValidationLayers();
			m_valiationLayers->Create(m_apiInstance);
		}

		void Renderer::Run()
		{

		}

		void Renderer::Destroy()
		{
			m_valiationLayers->Destroy(m_apiInstance);
			m_interface->DestroyValidationLayers(m_valiationLayers);

			m_apiInstance->Destroy();
			m_interface->DestroyInstance(m_apiInstance);

			delete m_interface;
		}
	}
}