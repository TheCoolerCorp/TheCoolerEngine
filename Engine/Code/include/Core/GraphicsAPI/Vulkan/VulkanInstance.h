#ifndef VKINSTANCE_H
#define VKINSTANCE_H

#include "EngineExport.h"

#include "Core/Interfaces/IInstance.h"


#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

namespace Engine::Core::GraphicsAPI
{
	class ENGINE_API VulkanInstance : public RHI::IInstance
	{
	public:
		~VulkanInstance() override = default;
		VulkanInstance* CastVulkan() override { return this; }

		void Create() override;
		void Destroy() override;

	private:
		VkInstance m_instance = VK_NULL_HANDLE;
	};
}


#endif 