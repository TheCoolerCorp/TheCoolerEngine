#include "GamePlay/Components/LightComponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType LightComponent::Create(int& a_outID)
		{
			a_outID = ServiceLocator::GetRendererSystem()->AddComponent(this);
			return ComponentType::LIGHT;
		}

		void LightComponent::SetLight(Light& a_light)
		{
			m_light = a_light;
		}
		void LightComponent::Destroy()
		{
			// Do nothing for now
		}

		LightComponent* LightComponent::GetComponent(int a_id)
		{
			return ServiceLocator::GetRendererSystem()->GetLightComponent(a_id);
		}

		void LightComponent::RemoveComponent(int a_id)
		{
			ServiceLocator::GetRendererSystem()->RemoveLightComponent(a_id);
		}
	}
}


