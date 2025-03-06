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
			class ENGINE_API VulkanInstance;
		}
		namespace RHI
		{
			class ENGINE_API IInstance
			{
			public:
				virtual ~IInstance() = default;
				virtual GraphicsAPI::VulkanInstance* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				virtual void Create() = 0;
				virtual void Destroy() = 0;
			};
		}
	}
}
#endif