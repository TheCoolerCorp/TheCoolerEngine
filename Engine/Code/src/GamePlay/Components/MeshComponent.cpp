#include  "GamePlay/Components/Meshcomponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType MeshComponent::Create(int* a_outId)
		{
			//ServiceLocator::m_meshsystem.Add(this);


			return ComponentType::MESH;
		}

		void MeshComponent::Update()
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

		void MeshComponent::SetTexture(Resource::Texture* a_texture)
		{
			// Set the texture to use.
			m_texture = a_texture;
		}
	}
}
