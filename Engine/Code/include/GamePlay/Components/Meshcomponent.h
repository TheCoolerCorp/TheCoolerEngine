#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include  "Ressources/Mesh.h"
#include "Ressources/Texture.h"
#include <string>

namespace Engine
{
	namespace GamePlay
	{
		class MeshComponent : public Component 
		{
		public:
			MeshComponent() = default;
			~MeshComponent() override = default;

			ENGINE_API ComponentType Create(uint32_t& a_outId, bool a_colliderMesh = false);

			ENGINE_API void Update();

			ENGINE_API void Destroy();

			ENGINE_API void SetMesh(Ref<Resource::Mesh> a_mesh);

			ENGINE_API void SetTexture(Ref<Resource::Texture> a_texture);

			ENGINE_API static ComponentType GetType(bool a_colliderMesh = false) { return a_colliderMesh ? ComponentType::COLLIDERMESH : ComponentType::MESH; }

			ENGINE_API static MeshComponent* GetComponent(int a_id);

			ENGINE_API static void RemoveComponent(int a_id);

			ENGINE_API Ref<Resource::Mesh> GetMesh() { return m_mesh; }
			ENGINE_API Ref<Resource::Texture> GetTexture() { return m_texture; }

			//static MeshComponent* FindComponent(int a_id)
		private:
			Ref<Resource::Mesh> m_mesh = nullptr;
			Ref<Resource::Texture> m_texture = nullptr;
		};
	}
}

#endif 