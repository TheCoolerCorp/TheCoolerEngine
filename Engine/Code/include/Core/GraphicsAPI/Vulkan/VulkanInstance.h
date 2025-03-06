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
			class ENGINE_API VulkanInstance : public RHI::IInstance
			{
			public:
				~VulkanInstance() override = default;
				VulkanInstance* CastVulkan() override { return this; }

				void Create() override;
				void Destroy() override;

				VkInstance GetVkInstance() { return m_Instance; }

			private:
				static std::vector<const char*> GetRequiredExtensions();

				VkInstance m_Instance = VK_NULL_HANDLE;
			};
		}
	}

}
#endif 