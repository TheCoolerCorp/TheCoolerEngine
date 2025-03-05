#ifndef IVALIDATIONLAYERS_H
#define IVALIDATIONLAYERS_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
#ifndef TCDEBUG
			constexpr bool ENABLEVALIDATIONLAYERS = false;
#else
			constexpr bool ENABLEVALIDATIONLAYERS = true;
#endif

			class VulkanValidationLayers;
			class IInstance;

			class ENGINE_API IValidationLayers
			{
			public:
				virtual ~IValidationLayers() = default;
				virtual void Create(IInstance* instance) = 0;
				virtual void Destroy(IInstance* instance) = 0;
				virtual VulkanValidationLayers* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanValidationLayers!"); return nullptr; }
			};
		}
	}
}

#endif