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

			ENGINE_API void Create(std::shared_ptr<Resource::Mesh> a_mesh);
			ENGINE_API void Destroy();

		private:
			std::shared_ptr<Resource::Mesh> m_mesh;
		};
	}
}

#endif 