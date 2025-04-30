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
			COLLIDERMESH = 4,
			LIGHT = 5
		};

		class Component
		{
		public:
			ENGINE_API virtual ~Component() = default;

			ENGINE_API virtual ComponentType Create(int& a_outId) = 0;
			ENGINE_API virtual void Destroy() = 0;
		};

	}
}

#endif 