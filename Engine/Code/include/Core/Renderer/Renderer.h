#ifndef RENDERER_H
#define RENDERER_H

#include "EngineExport.h"

#include "Core/Interfaces/ApiInterface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"

/*
* You can implement in your own way in this file
*
*
*/

namespace Engine
{
	namespace Core
	{
		class ENGINE_API Renderer
		{
		public:
			Renderer() = default;
			~Renderer() = default;

			void Init();
			void Run();
			void Destroy();

		private:
			RHI::ApiInterface* m_interface = nullptr;

			
		};
	}
}
#endif