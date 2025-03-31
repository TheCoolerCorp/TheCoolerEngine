#include  "GamePlay/Components/MaterialComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		void MaterialComponent::Create()
		{
			// Do nothing for now.
		}

		void MaterialComponent::Update()
		{
			// Do nothing for now.
		}

		void MaterialComponent::Destroy()
		{
			// Just set texture to nullptr, resource manager handle the resource.
			m_texture = nullptr;
		}

		void MaterialComponent::SetTexture(Resource::Texture* a_texture)
		{
			// Set texture to use.
			m_texture = a_texture;
		}
	}
}
