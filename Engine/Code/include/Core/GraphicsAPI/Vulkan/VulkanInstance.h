#ifndef VKINSTANCE_H
#define VKINSTANCE_H

#include "EngineExport.h"

#include "Core/Interfaces/IInstance.h"

#include  "vulkan/vulkan.h"

namespace Engine::Core::Window
{
	class IWindow;
}

namespace Engine::Core::GraphicsAPI
{
	class ENGINE_API VulkanInstance : public RHI::IInstance
	{
	public:
		~VulkanInstance() override = default;
		VulkanInstance* CastVulkan() override { return this; }

		void Create(Window::IWindow* a_window) override;
		void Destroy() override;

	private:
		VkInstance m_instance_ = VK_NULL_HANDLE;
	};
}


#endif 