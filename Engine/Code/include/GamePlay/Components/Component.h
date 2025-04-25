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
		enum class ComponentType : uint8_t
		{
			TRANSFORM = 0,
			MESH = 1,
			MATERIAL = 2,
			RIGIDBODY = 3,
			COLLIDERMESH = 4
		};

		class Component
		{
		public:
			virtual ~Component() = default;
		};

	}
}

#endif 