#ifndef RENDERER_H
#define RENDERER_H

#include "EngineExport.h"

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"
/*
* You can implement in your own way in this file
*
*
*/
#include "Ressources/Texture.h"
#include "Ressources/Mesh.h"

namespace Engine
{
	namespace Core
	{
		enum class RendererType
		{
			VULKAN = 0,
			OPENGL = 1,
			DX12 = 2
		};

		class Renderer
		{
		public:
			ENGINE_API Renderer() = default;
			ENGINE_API ~Renderer() = default;

			ENGINE_API void Init(RendererType a_type, Window::IWindow* a_window);
			ENGINE_API void Run(Window::IWindow* a_window);
			ENGINE_API void WaitIdle() const;
			ENGINE_API void Destroy();

			RHI::ApiInterface* GetApiInterface() const { return m_interface; }

			RHI::IInstance* GetApiInstance() const { return m_apiInstance; }
			RHI::IValidationLayers* GetValidationLayers() const { return m_valiationLayers; }

			RHI::IPhysicalDevice* GetPhysicalDevice() const { return m_physicalDevice; }
			RHI::ILogicalDevice* GetLogicalDevice() const { return m_logicalDevice; }

			RHI::ISurface* GetSurface() const { return m_surface; }

			RHI::ISwapChain* GetSwapChain() const { return m_swapChain; }
			RHI::IRenderPass* GetRenderPass() const { return m_renderPass; }
			RHI::IGraphicPipeline* GetGraphicPipeline() const { return m_graphicPipeline; }
			RHI::ICommandPool* GetCommandPool() const { return m_commandPool; }
			RHI::IDescriptorPool* GetDescriptorPool() const { return m_descriptorPool; }
		private:
			RendererType m_type = RendererType::VULKAN;

			RHI::ApiInterface* m_interface = nullptr;

			RHI::IInstance* m_apiInstance = nullptr;
		    RHI::IValidationLayers* m_valiationLayers = nullptr;

			RHI::IPhysicalDevice* m_physicalDevice = nullptr;
			RHI::ILogicalDevice* m_logicalDevice = nullptr;

			RHI::ISurface* m_surface = nullptr;

			RHI::ISwapChain* m_swapChain = nullptr;
			RHI::IRenderPass* m_renderPass = nullptr;
			RHI::IGraphicPipeline* m_graphicPipeline = nullptr;
			RHI::ICommandPool* m_commandPool = nullptr;
			RHI::IDescriptorPool* m_descriptorPool = nullptr;
			
		};
	}
}
#endif