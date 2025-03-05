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
			class ENGINE_API VulkanValidationLayers;
		}

		namespace RHI
		{
#ifndef TCDEBUG
			constexpr bool ENABLEVALIDATIONLAYERS = false;
#else
			constexpr bool ENABLEVALIDATIONLAYERS = true;
#endif

			class IInstance;

			class ENGINE_API IValidationLayers
			{
			public:
				virtual ~IValidationLayers() = default;
				virtual void Create(IInstance* instance) = 0;
				virtual void Destroy(IInstance* instance) = 0;
				//virtual VulkanValidationLayers* CastVulkan() { Core::Debugging::LOG_ERROR("Try to return wrong cast type : VulkanValidationLayers!"); return nullptr; }
				virtual GraphicsAPI::VulkanValidationLayers* CastVulkan() {
					Engine::Core::Debugging::LOG(Engine::Core::Debugging::LogLevel::DEBUGLOG, "Can't cast validationLayers", Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::YELLOW)); return nullptr; }
			};
		}
	}
}

#endif