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

			ENGINE_API ComponentType Create(uint32_t& a_outId);

			ENGINE_API void Update();

			ENGINE_API void Destroy();

			ENGINE_API void SetMesh(Resource::Mesh* a_mesh);

			ENGINE_API void SetTexture(Resource::Texture* a_texture);

			ENGINE_API static ComponentType GetType() { return ComponentType::MESH; }

			ENGINE_API static MeshComponent* GetComponent(uint32_t a_id);

			ENGINE_API static void RemoveComponent(uint32_t a_id);

			ENGINE_API Resource::Mesh* GetMesh() { return m_mesh; }
			ENGINE_API Resource::Texture* GetTexture() { return m_texture; }

			//static MeshComponent* FindComponent(uint32_t a_id)
		private:
			Resource::Mesh* m_mesh = nullptr;
			Resource::Texture* m_texture = nullptr;
		};
	}
}

#endif 