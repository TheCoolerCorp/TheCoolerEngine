#include  "GamePlay/Components/Meshcomponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType MeshComponent::Create(uint32_t& a_outId, bool a_colliderMesh)
		{
			a_outId = ServiceLocator::GetMeshRendererSystem()->AddComponent(this);
			return a_colliderMesh ? ComponentType::COLLIDERMESH : ComponentType::MESH;
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

		void MeshComponent::SetMesh(Ref<Resource::Mesh> a_mesh)
		{
			// Set the mesh to use.
			m_mesh = a_mesh;
		}

		void MeshComponent::SetTexture(Ref<Resource::Texture> a_texture)
		{
			// Set the texture to use.
			m_texture = a_texture;
		}

		MeshComponent* MeshComponent::GetComponent(uint32_t a_id)
		{
			return ServiceLocator::GetMeshRendererSystem()->GetComponent(a_id);
		}

		void MeshComponent::RemoveComponent(uint32_t a_id)
		{
			ServiceLocator::GetMeshRendererSystem()->RemoveComponent(a_id);
		}
	}
}
