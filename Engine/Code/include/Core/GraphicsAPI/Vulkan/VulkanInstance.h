#ifndef VKINSTANCE_H
#define VKINSTANCE_H

#include <vector>

#include "EngineExport.h"

#include "Core/Interfaces/IInstance.h"


#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanInstance : public RHI::IInstance
			{
			public:
				ENGINE_API ~VulkanInstance() override = default;
				ENGINE_API VulkanInstance* CastVulkan() override { return this; }

				ENGINE_API void Create() override;
				ENGINE_API void Destroy() override;

				ENGINE_API VkInstance GetVkInstance() { return m_instance; }

			private:
				ENGINE_API static std::vector<const char*> GetRequiredExtensions();

				VkInstance m_instance = VK_NULL_HANDLE;
			};
		}
	}

}
#endif 