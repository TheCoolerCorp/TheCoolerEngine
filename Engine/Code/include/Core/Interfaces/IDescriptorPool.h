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
			class ENGINE_API VulkanDescriptorPool;
		}

		namespace RHI
		{
			class ILogicalDevice;

			class ENGINE_API IDescriptorPool
			{
			public:
				virtual ~IDescriptorPool() = default;
				virtual GraphicsAPI::VulkanDescriptorPool* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				virtual void Create(ILogicalDevice* a_logicalDevice, int a_maxFrameInFlight) = 0;
				virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif