#ifndef LIGHTGO_H
#define LIGHTGO_H

#include "EngineExport.h"

#include "Core/Logger/Logger.h"
#include "GameObject.h"

namespace Engine
{
	namespace GamePlay
	{
		class LightGO : public GameObject
		{
		public:
			ENGINE_API LightGO(std::string a_name = "Light");
			ENGINE_API LightGO(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name = "Light");

			ENGINE_API ~LightGO() override;

			ENGINE_API LightComponent* GetLightComponents() { return GetComponent<LightComponent>(); }
		};


	}
}

#endif 