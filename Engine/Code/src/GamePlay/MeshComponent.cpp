#include  "GamePlay/Meshcomponent.h"

namespace Engine
{
	namespace GamePlay
	{
		void MeshComponent::Create(std::string a_path)
		{
			m_mesh = dynamic_cast<Resource::Mesh*>(Resource::ResourceManager::Get().CreateResourceReference(Resource::ResourceType::MESH, a_path, "mesh"));
			
		}
		void MeshComponent::Destroy()
		{
			if (m_mesh)
			{
				Resource::ResourceManager::Get().DestroyResource("mesh");
			}
		}
	}
}
