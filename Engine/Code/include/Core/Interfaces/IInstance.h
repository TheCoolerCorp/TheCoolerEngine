#ifndef IINSTANCE_H
#define IINSTANCE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanInstance;
		}
		namespace RHI
		{
			class IInstance
			{
			public:
				ENGINE_API virtual ~IInstance() = default;
				ENGINE_API virtual GraphicsAPI::VulkanInstance* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create() = 0;
				ENGINE_API virtual void Destroy() = 0;
			};
		}
	}
}
#endif