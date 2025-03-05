#ifndef IINSTANCE_H
#define IINSTANCE_H

#include "EngineExport.h"

#include "Core/Window/IWindow.h"

namespace Engine::Core::GraphicsAPI
{
	class ENGINE_API VulkanInstance;
}

namespace Engine::Core::RHI
{

	class IInstance
	{
	public:
		virtual ~IInstance() = default;
		virtual GraphicsAPI::VulkanInstance* CastVulkan() { return nullptr; }

		virtual void Create(Window::IWindow* a_window) = 0;
		virtual void Destroy() = 0;
	};
}
#endif