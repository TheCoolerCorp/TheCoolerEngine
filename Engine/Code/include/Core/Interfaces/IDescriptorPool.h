#ifndef IDESCRIPTORPOOL_h
#define IDESCRIPTORPOOL_h

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanDescriptorPool;
		}

		namespace RHI
		{
			class ILogicalDevice;

			class IDescriptorPool
			{
			public:
				ENGINE_API virtual ~IDescriptorPool() = default;
				ENGINE_API virtual GraphicsAPI::VulkanDescriptorPool* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(ILogicalDevice* a_logicalDevice, int a_maxFrameInFlight) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif