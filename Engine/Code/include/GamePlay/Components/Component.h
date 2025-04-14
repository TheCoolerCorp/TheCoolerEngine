#ifndef COMPONENT_H
#define COMPONENT_H

#include "EngineExport.h"

#include <iostream>
#include <string>
#include <filesystem>
//#include <Refureku/Refureku.h>
//#include "Component.rfkh.h"

namespace Engine
{
	namespace GamePlay
	{
		enum class ComponentType
		{
			TRANSFORM = 0,
			MESH = 1,
			MATERIAL = 2
		};

		class Component
		{
		public:
			ENGINE_API virtual ~Component() = default;

			ENGINE_API virtual ComponentType Create(uint32_t& a_outId) = 0;
			ENGINE_API virtual void Destroy() = 0;
		};

	}
}

#endif 