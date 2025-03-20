#include "GamePlay/GameObject.h"

#include "GamePlay/Meshcomponent.h"
#include  "GamePlay/TextureComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		GameObject::GameObject(Math::vec3 a_position, Math::quat a_rotation, Math::vec3 a_scale)
		{
			m_transform = Math::Transform(a_position, a_rotation, a_scale);
		}

		void GameObject::Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info)
		{
			m_descriptor = a_interface->InstantiateObjectDescriptor();
			m_descriptor->Create(a_info.a_logicalDevice, a_info.a_physicalDevice, a_info.a_graphicPipeline, a_info.a_descriptorPool, a_info.a_commandPool, this, a_info.size);
		}
	}
}