#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include "GamePlay/ComponentsBase/Light.h"

namespace Engine
{
	namespace GamePlay
	{
		class LightComponent : public Component
		{
		public:
			ENGINE_API ~LightComponent() override = default;

			ENGINE_API ComponentType Create(int& a_outID) override;
			ENGINE_API void Destroy() override;

			ENGINE_API Light& GetLight() { return m_light; }

		private:
			Light m_light;
		};
	}
}


#endif
