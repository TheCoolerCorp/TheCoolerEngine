#include  "GamePlay/Meshcomponent.h"

namespace Engine
{
	namespace GamePlay
	{
		void MeshComponent::Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool)
		{
			ComputeName(a_path);
			m_mesh = dynamic_cast<Resource::Mesh*>(Resource::ResourceManager::Get().CreateResourceReference(Resource::ResourceType::MESH, a_path, m_name, a_interface, a_physicalDevice, a_logicalDevice, a_commandPool));
		}

		void MeshComponent::Destroy(Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			if (m_mesh)
			{
				Resource::ResourceManager::Get().DestroyResource(m_name, a_logicalDevice);
			}
		}
	}
}
