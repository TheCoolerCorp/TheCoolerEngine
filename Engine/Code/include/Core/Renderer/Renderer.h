#ifndef RENDERER_H
#define RENDERER_H

#include "EngineExport.h"

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"
/*
* You can implement in your own way in this file
*
*
*/

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

		class ENGINE_API Renderer
		{
		public:
			Renderer() = default;
			~Renderer() = default;

			void Init(RendererType a_type, Window::IWindow* a_window);
			void Run();
			void Destroy();

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
			
		};
	}
}
#endif