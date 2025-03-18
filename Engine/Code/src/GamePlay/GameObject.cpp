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
	}
}