#ifndef IVALIDATIONLAYERS_H
#define IVALIDATIONLAYERS_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanValidationLayers;
		}

		namespace RHI
		{
#ifndef TCDEBUG
			constexpr bool ENABLEVALIDATIONLAYERS = false;
#else
			constexpr bool ENABLEVALIDATIONLAYERS = true;
#endif

			class IInstance;

			class IValidationLayers
			{
			public:
				ENGINE_API virtual ~IValidationLayers() = default;
				ENGINE_API 	virtual void Create(IInstance* instance) = 0;
				ENGINE_API virtual void Destroy(IInstance* instance) = 0;
				ENGINE_API virtual GraphicsAPI::VulkanValidationLayers* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanValidationLayers!"); return nullptr; }
			};
		}
	}
}

#endif