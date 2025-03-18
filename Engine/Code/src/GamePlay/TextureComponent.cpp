#include  "GamePlay/TextureComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		void TextureComponent::Create(std::string a_path)
		{
			m_texture = dynamic_cast<Resource::Texture*>(Resource::ResourceManager::Get().CreateResourceReference(Resource::ResourceType::MESH, a_path, "mesh"));

		}
		void TextureComponent::Destroy()
		{
			if (m_texture)
			{
				Resource::ResourceManager::Get().DestroyResource("mesh");
			}
		}
	}
}
