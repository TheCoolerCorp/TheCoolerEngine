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


namespace Engine::Core::Renderer
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
		// Interface to instanciate your objects
		RHI::ApiInterface* m_apiInterface = nullptr;
			
		// Objects
		RHI::IInstance* m_instance = nullptr;
	};
}

#endif