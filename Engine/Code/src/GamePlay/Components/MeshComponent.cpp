#include  "GamePlay/Components/Meshcomponent.h"

#include <utility>
#include <meta/factory.hpp>
#include <meta/meta.hpp>

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		void MeshComponent::Register()
		{
			constexpr std::hash<std::string_view> t_hash{};

			meta::reflect<MeshData>(t_hash("MeshData"))
				.data<&MeshData::mMeshPath>(t_hash("mesh"))
				.data<&MeshData::mAlbedoPath>(t_hash("albedo"))
				.data<&MeshData::mNormalPath>(t_hash("normal"))
				.data<&MeshData::mMetallicPath>(t_hash("metallic"))
				.data<&MeshData::mRoughnessPath>(t_hash("roughness"))
				.data<&MeshData::mAOPath>(t_hash("ao"));

			meta::reflect<MeshComponent>(t_hash("MeshComponent"))
				.data<&MeshComponent::SetFromData, &MeshComponent::GetData>(t_hash("Mesh"));
		}

		ComponentType MeshComponent::Create(int& a_outId)
		{
			m_material = CreateRef<Material>();

			a_outId = ServiceLocator::GetRendererSystem()->AddComponent(this);
			return ComponentType::MESH;

		}

		void MeshComponent::Destroy()
		{
			// Just set mesh to nullptr, resource manager handle the resource.
			m_mesh = nullptr;
			m_material = nullptr;
		}

		void MeshComponent::SetMesh(const std::string& a_path, Core::Renderer* a_renderer)
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Mesh> t_mesh = t_resourceManager->CreateResource<Resource::Mesh>(a_path);
			t_mesh->Load(a_renderer);

			// Set the mesh to use.
			m_mesh = std::move(t_mesh);
		}

		void MeshComponent::SetMaterial(Ref<Material> a_material)
		{
			// Set the material to use.
			m_material = a_material;
		}

		MeshComponent* MeshComponent::GetComponent(int a_id)
		{
			return ServiceLocator::GetRendererSystem()->GetMeshComponent(a_id);
		}

		MeshData MeshComponent::GetData()
		{
			Ref<Resource::Texture> t_albedoTexture = m_material->GetAlbedo();
			std::string t_albedoPath = t_albedoTexture ? t_albedoTexture->GetPath() : "";

			Ref<Resource::Texture> t_normalTexture = m_material->GetNormal();
			std::string t_normalPath = t_normalTexture ? t_normalTexture->GetPath() : "";

			Ref<Resource::Texture> t_metallicTexture = m_material->GetMetallic();
			std::string t_metallicPath = t_metallicTexture ? t_metallicTexture->GetPath() : "";

			Ref<Resource::Texture> t_roughnessTexture = m_material->GetRoughness();
			std::string t_roughnessPath = t_roughnessTexture ? t_roughnessTexture->GetPath() : "";

			Ref<Resource::Texture> t_AOTexture = m_material->GetAO();
			std::string t_AOPath = t_AOTexture ? t_AOTexture->GetPath() : "";

			MeshData t_data = {
				.mMeshPath= m_mesh->GetPath(),
				.mAlbedoPath= t_albedoPath,
				.mNormalPath= t_normalPath,
				.mMetallicPath= t_metallicPath,
				.mRoughnessPath= t_roughnessPath,
				.mAOPath= t_AOPath
			};

			return t_data;
		}

		void MeshComponent::RemoveComponent(int a_id)
		{
			ServiceLocator::GetRendererSystem()->RemoveMeshComponent(a_id);
		}
	}
}
