#include  "GamePlay/Meshcomponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		void MeshComponent::Create(std::string a_path)
		{
			m_mesh = ServiceLocator::GetResourceManager()->CreateResource<Resource::Mesh>(a_path);
		}

		/*void MeshComponent::Destroy(Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			if (m_mesh)
			{
				Resource::ResourceManager::Get().DestroyResource(m_name, a_logicalDevice);
			}
		}*/
	}
}
