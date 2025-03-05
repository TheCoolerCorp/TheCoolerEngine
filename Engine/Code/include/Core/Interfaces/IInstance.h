#ifndef IINSTANCE_H
#define IINSTANCE_H

#include "EngineExport.h"


namespace Engine::Core::GraphicsAPI
{
	class ENGINE_API VulkanInstance;
}

namespace Engine::Core::RHI
{

	class ENGINE_API IInstance
	{
	public:
		virtual ~IInstance() = default;
		virtual GraphicsAPI::VulkanInstance* CastVulkan() { return nullptr; }

		virtual void Create() = 0;
		virtual void Destroy() = 0;
	};
}
#endif