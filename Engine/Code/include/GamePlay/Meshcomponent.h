#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include "Ressources/ResourceManager.h"
#include  "Ressources/Mesh.h"
namespace Engine
{
	namespace GamePlay
	{
		class MeshComponent : public Component 
		{
		public:
			ENGINE_API void Create(std::string a_path) override;
			ENGINE_API void Destroy() override;


		private:
			Resource::Mesh* m_mesh = nullptr;
			
		};
	}
}

#endif 