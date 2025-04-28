#include <utility>

#include  "GamePlay/Components/Meshcomponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType MeshComponent::Create(int& a_outId)
		{
			m_material = CreateRef<Material>();

			a_outId = ServiceLocator::GetMeshRendererSystem()->AddComponent(this);
			return ComponentType::MESH;

		}

		void MeshComponent::Destroy()
		{
			// Just set mesh to nullptr, resource manager handle the resource.
			m_mesh = nullptr;
			m_material = nullptr;
		}

		void MeshComponent::SetMesh(Ref<Resource::Mesh> a_mesh)
		{
			// Set the mesh to use.
			m_mesh = std::move(a_mesh);
		}

		void MeshComponent::SetMaterial(Ref<Material> a_material)
		{
			// Set the material to use.
			m_material = a_material;
		}

		MeshComponent* MeshComponent::GetComponent(int a_id)
		{
			return ServiceLocator::GetMeshRendererSystem()->GetComponent(a_id);
		}

		void MeshComponent::RemoveComponent(int a_id)
		{
			ServiceLocator::GetMeshRendererSystem()->RemoveComponent(a_id);
		}
	}
}
