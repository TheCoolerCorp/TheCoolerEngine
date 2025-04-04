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
			virtual ~Component() = default;
		};

	}
}

#endif 