#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "EngineExport.h"

#include "System.h"

namespace Engine
{
	namespace GamePlay
	{
		class PhysicsSystem : System
		{
		public:
			ENGINE_API PhysicsSystem() = default;
			ENGINE_API ~PhysicsSystem() override = default;
		};
	}
}

#endif