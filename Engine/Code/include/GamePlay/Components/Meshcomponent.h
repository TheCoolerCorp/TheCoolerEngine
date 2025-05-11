#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include  "Ressources/Mesh.h"
#include "GamePlay/ComponentsBase/Material.h"
#include "Ressources/Texture.h"
#include <string>

namespace Engine
{
	namespace GamePlay
	{
		struct MeshData
		{
			std::string mMeshPath;
			int mType;
			std::string mAlbedoPath;
			std::string mNormalPath;
			std::string mMetallicPath;
			std::string mRoughnessPath;
			std::string mAOPath;
		};

		class MeshComponent : public Component 
		{
		public:
			ENGINE_API MeshComponent() = default;
			ENGINE_API ~MeshComponent() override = default;

			ENGINE_API static void Register();
			ENGINE_API ComponentType Create(int& a_outId) override;

			ENGINE_API void Destroy() override;

			ENGINE_API void SetMesh(const std::string& a_path, Core::Renderer* a_renderer);

			ENGINE_API void SetFromData(const MeshData& a_data) {}

			ENGINE_API void SetMaterial(Ref<Material> a_material);

			ENGINE_API static ComponentType GetType() { return ComponentType::MESH; }

			ENGINE_API static MeshComponent* GetComponent(int a_id);

			ENGINE_API MeshData GetData();

			ENGINE_API static void RemoveComponent(int a_id);

			ENGINE_API Ref<Resource::Mesh> GetMesh() { return m_mesh; }
			ENGINE_API Ref<Material> GetMaterial() { return m_material; }
			//static MeshComponent* FindComponent(uint32_t a_id)

		private:

			Ref<Resource::Mesh> m_mesh = nullptr;
			Ref<Material> m_material = nullptr;
		};
	}
}

#endif 