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

			ENGINE_API static void Register();
			ENGINE_API ComponentType Create(int& a_outID) override;
			ENGINE_API void SetLight(Light& a_light);
			ENGINE_API void SetLightFromData(const LightData& a_lightData);
			ENGINE_API void Destroy() override;

			[[nodiscard]] ENGINE_API Light& GetLight() { return m_light; }
			[[nodiscard]] ENGINE_API LightData GetData();

			ENGINE_API static ComponentType GetType() { return ComponentType::LIGHT; }
			
			ENGINE_API static LightComponent* GetComponent(int a_id);

			ENGINE_API static void RemoveComponent(int a_id);

		private:
			Light m_light;
		};
	}
}


#endif
