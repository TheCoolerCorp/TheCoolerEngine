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

			ENGINE_API ComponentType Create(int* a_outId);

			ENGINE_API void Update();

			ENGINE_API void Destroy();

			ENGINE_API void SetMesh(Resource::Mesh* a_mesh);

			ENGINE_API void SetTexture(Resource::Texture* a_texture);

			Resource::Mesh* GetMesh() { return m_mesh; }
			Resource::Texture* GetTexture() { return m_texture; }

		private:
			Resource::Mesh* m_mesh = nullptr;
			Resource::Texture* m_texture = nullptr;
		};
	}
}

#endif 