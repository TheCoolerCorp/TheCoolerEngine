#include  "GamePlay/Meshcomponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		void MeshComponent::Create(std::shared_ptr<Resource::Mesh> a_mesh)
		{
			m_mesh = a_mesh;
		}

		void MeshComponent::Destroy()
		{
			/*if (m_mesh)
			{
				Resource::ResourceManager::Get().DestroyResource(m_name, a_logicalDevice);
			}*/
		}
	}
}
