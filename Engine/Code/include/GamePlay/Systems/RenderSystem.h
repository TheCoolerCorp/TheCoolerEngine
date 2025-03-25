#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "EngineExport.h"

#include "System.h"

namespace Engine
{
	namespace GamePlay
	{
		class RenderSystem : public System
		{
		public:
			ENGINE_API RenderSystem() = default;
			ENGINE_API ~RenderSystem() = default;

			ENGINE_API void Update(ComponentsPool& a_componentsPool) override;

		};
	}
}
#endif