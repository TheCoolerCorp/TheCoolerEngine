#include "GamePlay/ComponentsBase/Material.h"

#include "GamePlay/ServiceLocator.h"
#include "Ressources/ResourceManager.h"

namespace Engine
{
	namespace GamePlay
	{

		Material::Material()
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Texture> t_defaultTexture = t_resourceManager->CreateResource<Resource::Texture>(DefaultMaterial);
			m_empty = t_defaultTexture;
		}

		Material::Material(MaterialType a_type) : m_type(a_type)
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Texture> t_defaultTexture = t_resourceManager->CreateResource<Resource::Texture>(DefaultMaterial);
			m_empty = t_defaultTexture;
		}

		void Material::SetAlbedo(const std::string& a_path, Core::Renderer* a_renderer)
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Texture> t_albedoTexture = t_resourceManager->CreateResource<Resource::Texture>(a_path);
			t_albedoTexture->Load(a_renderer);

			m_textures[0] = t_albedoTexture;
			m_hasTextures.albdeo = true;

			if (!m_empty->IsLoaded())
			{
				m_empty->Load(a_renderer);
			}
		}

		void Material::SetAlbedo(Math::vec3 a_albedoValue)
		{
			m_values.albedo = a_albedoValue;
		}

		void Material::RemoveAlbedo()
		{
			m_textures[0] = nullptr;
			m_hasTextures.albdeo = false;
		}

		void Material::SetNormal(const std::string& a_path, Core::Renderer* a_renderer)
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Texture> t_normalTexture = t_resourceManager->CreateResource<Resource::Texture>(a_path);
			t_normalTexture->Load(a_renderer);

			m_textures[1] = t_normalTexture;
			m_hasTextures.normal = true;
		}

		void Material::RemoveNormal()
		{
			m_textures[1] = nullptr;
			m_hasTextures.normal = false;
		}

		void Material::SetMetallic(const std::string& a_path, Core::Renderer* a_renderer)
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Texture> t_metallicTexture = t_resourceManager->CreateResource<Resource::Texture>(a_path);
			t_metallicTexture->Load(a_renderer);

			m_textures[2] = t_metallicTexture;
			m_hasTextures.metallic = true;
		}

		void Material::SetMetallic(float a_metallicValue)
		{
			m_values.metallic = a_metallicValue;
		}

		void Material::RemoveMetallic()
		{
			m_textures[2] = nullptr;
			m_hasTextures.metallic = false;
		}

		void Material::SetRoughness(const std::string& a_path, Core::Renderer* a_renderer)
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Texture> t_roughnessTexture = t_resourceManager->CreateResource<Resource::Texture>(a_path);
			t_roughnessTexture->Load(a_renderer);

			m_textures[3] = t_roughnessTexture;
			m_hasTextures.roughness = true;
		}
		void Material::SetRoughness(float a_roughnessValue)
		{
			m_values.roughness = a_roughnessValue;
		}

		void Material::RemoveRoughness()
		{
			m_textures[3] = nullptr;
			m_hasTextures.roughness = false;
		}

		void Material::SetAO(const std::string& a_path, Core::Renderer* a_renderer)
		{
			Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();

			Ref<Resource::Texture> t_aoTexture = t_resourceManager->CreateResource<Resource::Texture>(a_path);
			t_aoTexture->Load(a_renderer);

			m_textures[4] = t_aoTexture;
			m_hasTextures.ao = true;
		}

		void Material::SetAO(float a_aoValue)
		{
			m_values.ao = a_aoValue;
		}

		void Material::RemoveAO()
		{
			m_textures[4] = nullptr;
			m_hasTextures.ao = false;
		}
	}
}
