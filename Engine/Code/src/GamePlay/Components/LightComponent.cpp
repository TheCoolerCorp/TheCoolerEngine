#include "GamePlay/Components/LightComponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType LightComponent::Create(int& a_outID)
		{
			//a_outID = ServiceLocator::GetMeshRendererSystem()->AddComponent(this);
			return ComponentType::MESH;
		}

		void LightComponent::Destroy()
		{
			// Do nothing for now
		}
	}
}


