#include  "GamePlay/Meshcomponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		void MeshComponent::Create()
		{
			// Do nothing for now.
		}

		void MeshComponent::Destroy()
		{
			// Just set mesh to nullptr, resource manager handle the resource.
			m_mesh = nullptr;
		}

		void MeshComponent::SetMesh(Resource::Mesh* a_mesh)
		{
			// Set the mesh to use.
			m_mesh = a_mesh;
		}
	}
}
