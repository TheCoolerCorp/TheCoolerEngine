#include  "GamePlay/TextureComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		void TextureComponent::Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool)
		{
			ComputeName(a_path);
			m_texture = dynamic_cast<Resource::Texture*>(Resource::ResourceManager::Get().CreateResourceReference(Resource::ResourceType::TEXTURE, a_path, m_name, a_interface, a_physicalDevice, a_logicalDevice, a_commandPool));

		}
		void TextureComponent::Destroy(Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			if (m_texture)
			{
				Resource::ResourceManager::Get().DestroyResource(m_name, a_logicalDevice);
			}
		}
	}
}
