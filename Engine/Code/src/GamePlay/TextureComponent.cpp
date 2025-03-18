#include  "GamePlay/TextureComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		void TextureComponent::Create(std::string a_path)
		{
			ComputeName(a_path);
			m_texture = dynamic_cast<Resource::Texture*>(Resource::ResourceManager::Get().CreateResourceReference(Resource::ResourceType::TEXTURE, a_path, m_name));

		}
		void TextureComponent::Destroy()
		{
			if (m_texture)
			{
				Resource::ResourceManager::Get().DestroyResource(m_name);
			}
		}
	}
}
