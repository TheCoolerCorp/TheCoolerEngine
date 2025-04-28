#include "GamePlay/ComponentsBase/Material.h"

namespace Engine
{
	namespace GamePlay
	{
		void Material::SetAlbedo(Ref<Resource::Texture> a_albedoTexture)
		{
			m_textures[0] = a_albedoTexture;

			if (!m_hasTextures.albdeo)
			{
				m_hasTextures.albdeo = true;
			}
		}

		void Material::SetAlbedo(Math::vec3 a_albedoValue)
		{
			m_values.albedo = a_albedoValue;
		}

		void Material::RemoveAlbedo()
		{
			m_textures[0] = nullptr;

			if (m_hasTextures.albdeo)
			{
				m_hasTextures.albdeo = false;
			}
		}

		void Material::SetNormal(Ref<Resource::Texture> a_normalTexture)
		{
			m_textures[1] = a_normalTexture;

			if (!m_hasTextures.normal)
			{
				m_hasTextures.normal = true;
			}
		}

		void Material::RemoveNormal()
		{
			m_textures[1] = nullptr;

			if (m_hasTextures.normal)
			{
				m_hasTextures.normal = false;
			}
		}

		void Material::SetMetallic(Ref<Resource::Texture> a_metallicTexture)
		{
			m_textures[2] = a_metallicTexture;

			if (!m_hasTextures.metallic)
			{
				m_hasTextures.metallic = true;
			}
		}

		void Material::SetMetallic(float a_metallicValue)
		{
			m_values.metallic = a_metallicValue;
		}

		void Material::RemoveMetallic()
		{
			m_textures[2] = nullptr;

			if (m_hasTextures.metallic)
			{
				m_hasTextures.metallic = false;
			}
		}

		void Material::SetRoughness(Ref<Resource::Texture> a_roughnessTexture)
		{
			m_textures[3] = a_roughnessTexture;

			if (!m_hasTextures.roughness)
			{
				m_hasTextures.roughness = true;
			}
		}
		void Material::SetRoughness(float a_roughnessValue)
		{
			m_values.roughness = a_roughnessValue;
		}

		void Material::RemoveRoughness()
		{
			m_textures[3] = nullptr;

			if (m_hasTextures.roughness)
			{
				m_hasTextures.roughness = false;
			}
		}

		void Material::SetAO(Ref<Resource::Texture> a_aoTexture)
		{
			m_textures[4] = a_aoTexture;

			if (!m_hasTextures.ao)
			{
				m_hasTextures.ao = true;
			}
		}

		void Material::SetAO(float a_aoValue)
		{
			m_values.ao = a_aoValue;
		}

		void Material::RemoveAO()
		{
			m_textures[4] = nullptr;

			if (m_hasTextures.ao)
			{
				m_hasTextures.ao = false;
			}
		}
	}
}