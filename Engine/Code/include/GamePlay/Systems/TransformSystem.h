#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include "EngineExport.h"

#include "System.h"

namespace Engine
{
	namespace GamePlay
	{
		class TransformSystem : public System
		{
		public:
			ENGINE_API TransformSystem() = default;
			ENGINE_API ~TransformSystem() = default;

			ENGINE_API void Update(ComponentsPool& a_componentsPool) override;

		};
	}
}
#endif