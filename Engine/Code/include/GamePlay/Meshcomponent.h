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
			ENGINE_API void Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool) override;
			ENGINE_API void Destroy(Core::RHI::ILogicalDevice* a_logicalDevice) override;

			Resource::Mesh* GetMesh() const { return m_mesh; }

		private:
			Resource::Mesh* m_mesh = nullptr;
			
		};
	}
}

#endif 