#include "GamePlay/Components/LightComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType LightComponent::Create(uint32_t& a_outID)
		{
			return ComponentType::MATERIAL;
		}

		void LightComponent::Destroy()
		{
			/*
			 * Do nothing for now
			 */
		}

	}
}