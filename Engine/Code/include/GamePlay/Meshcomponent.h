#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include  "Ressources/Mesh.h"

#include <string>

namespace Engine
{
	namespace GamePlay
	{
		class MeshComponent : public Component 
		{
		public:
			~MeshComponent() override = default;

			ENGINE_API void Create();
			ENGINE_API void Destroy();

			ENGINE_API void SetMesh(Resource::Mesh* a_mesh);

		private:
			Resource::Mesh* m_mesh = nullptr;
		};
	}
}

#endif 