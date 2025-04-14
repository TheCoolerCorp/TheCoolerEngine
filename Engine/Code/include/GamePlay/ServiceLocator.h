#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "EngineExport.h"

#include "Ressources/ResourceManager.h"
#include "GamePlay/Systems/TransformSystem.h"
#include "GamePlay/Systems/MeshRendererSystem.h"
#include "GamePlay/Systems/PhysicsSystem.h"

namespace Engine
{
	namespace GamePlay
	{
		class ServiceLocator
		{
		public:
			static void ProvideResourceManager(Resource::ResourceManager* a_resourceManager);
			static void ProvideTransformSystem(TransformSystem* a_transformSystem);
			static void ProvideRendererSystem(MeshRendererSystem* a_meshRendererSystem);
			static void ProvidePhysicsSystem(PhysicsSystem* a_physicsSystem);

			static Resource::ResourceManager* GetResourceManager();
			static TransformSystem* GetTransformSystem();
			static MeshRendererSystem* GetMeshRendererSystem();
			static PhysicsSystem* GetPhysicsSystem();

		private:
			static Resource::ResourceManager* m_resourceManager;
			static TransformSystem* m_transformSystem;
			static MeshRendererSystem* m_meshRendererSystem;
			static PhysicsSystem* m_physicsSystem;
		};
	}
}

#endif