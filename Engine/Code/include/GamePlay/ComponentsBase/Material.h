#ifndef MATERIAL_H
#define MATERIAL_H

#include "EngineExport.h"
#include "Ressources/Texture.h"
#include <vector>
#include "Math/vec3.h"

namespace Engine
{
	namespace GamePlay
	{
		#define DefaultMaterial std::string("Assets/Textures/DefaultTexture.png")
		/* When uploading material textures into descriptor for vulkan push at index + 1 (Index 0 is use by the model matrix) */
		/*
 		 *  0 = albedo
		 *	1 = normal
		 *	2 = metallic
		 *	3 = roughness
		 *	4 = ao
		 *
		 *	5 = texturesValue (upload all at once in a struct)
		 */


		/* metallic is just 1 of 0, no intermediate value like 0.5, 0.7, etc... */
		struct MaterialValues
		{
		public:
			Math::vec3 albedo = {0.5f, 0.5f, 0.5f};
			float metallic = 0.f;
			float roughness = 0.5f;
			float ao = 1.f;
		};

		struct HasMaterialTextures
		{
			uint32_t albdeo = 0;
			uint32_t normal = 0;
			uint32_t metallic = 0;
			uint32_t roughness = 0;
			uint32_t ao = 0;
		};

		enum MaterialType
		{
			UNLIT = 0,
			LIT = 1,
			SKYBOX = 2
		};

		class Material
		{
		public:
			ENGINE_API Material();
			ENGINE_API Material(MaterialType a_type);
			ENGINE_API ~Material() = default;

			void Create(MaterialType a_type) { m_type = a_type; }
			void Destroy();


			ENGINE_API void SetAlbedo(const std::string& a_path, Core::Renderer* a_renderer);
			ENGINE_API void SetAlbedo(const Ref<Resource::Texture>& a_albedoTexture);
			ENGINE_API void SetAlbedo(Math::vec3 a_albedoValue);
			ENGINE_API void RemoveAlbedo();

			/* If there is no normal texture, compute in shader base on mesh */
			ENGINE_API void SetNormal(const std::string& a_path, Core::Renderer* a_renderer);
			ENGINE_API void RemoveNormal();

			ENGINE_API void SetMetallic(const std::string& a_path, Core::Renderer* a_renderer);
			ENGINE_API void SetMetallic(float a_metallicValue);
			ENGINE_API void RemoveMetallic();

			ENGINE_API void SetRoughness(const std::string& a_path, Core::Renderer* a_renderer);
			ENGINE_API void SetRoughness(float a_roughnessValue);
			ENGINE_API void RemoveRoughness();

			ENGINE_API void SetAO(const std::string& a_path, Core::Renderer* a_renderer);
			ENGINE_API void SetAO(float a_aoValue);
			ENGINE_API void RemoveAO();

			ENGINE_API bool HasAlbedo() { return m_hasTextures.albdeo; }
			ENGINE_API Ref<Resource::Texture> GetAlbedo() { return m_textures[0]; }

			ENGINE_API bool HasNormal() { return m_hasTextures.normal; }
			ENGINE_API Ref<Resource::Texture> GetNormal() { return m_textures[1]; }

			ENGINE_API bool HasMetallic() { return m_hasTextures.metallic; }
			ENGINE_API Ref<Resource::Texture> GetMetallic() { return m_textures[2]; }

			ENGINE_API bool HasRoughness() { return m_hasTextures.roughness; }
			ENGINE_API Ref<Resource::Texture> GetRoughness() { return m_textures[3]; }

			ENGINE_API bool HasAO() { return m_hasTextures.ao; }
			ENGINE_API Ref<Resource::Texture> GetAO() { return m_textures[4]; }

			ENGINE_API Ref<Resource::Texture> GetEmpty() { return m_empty; }

			void SetType(MaterialType a_type) { m_type = a_type; m_needUpdate = true; }
			MaterialType GetType() { return m_type; }
			bool GetNeedUpdate() { return m_needUpdate; }
			void SetNeedUpdate(bool a_bool) { m_needUpdate = a_bool; }
			HasMaterialTextures& GetHasTextures() { return m_hasTextures; }
			MaterialValues& GetMaterialValues() { return m_values; }
		private:
			MaterialType m_type = UNLIT;

			std::vector<Ref<Resource::Texture>> m_textures = std::vector<Ref<Resource::Texture>>(5);
			MaterialValues m_values;
			HasMaterialTextures m_hasTextures;
			Ref<Resource::Texture> m_empty;
			bool m_needUpdate = true;
		};
	}
}

#endif