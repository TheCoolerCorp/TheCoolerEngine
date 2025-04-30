#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "EngineExport.h"

#include "Ressources/ResourceManager.h"
#include "GamePlay/Systems/TransformSystem.h"
#include "GamePlay/Systems/RenderSystem.h"
#include "GamePlay/Systems/PhysicsSystem.h"

namespace Engine
{
	namespace GamePlay
	{
		class ServiceLocator
		{
		public:
			ENGINE_API static void ProvideResourceManager(Resource::ResourceManager* a_resourceManager);
			static void ProvideTransformSystem(TransformSystem* a_transformSystem);
			static void ProvideRenderSystem(RenderSystem* a_meshRendererSystem);
			static void ProvidePhysicsSystem(PhysicsSystem* a_physicsSystem);

			ENGINE_API static Resource::ResourceManager* GetResourceManager();
			ENGINE_API static TransformSystem* GetTransformSystem();
			ENGINE_API static RenderSystem* GetRendererSystem();
			ENGINE_API static PhysicsSystem* GetPhysicsSystem();

		private:
			static Resource::ResourceManager* m_resourceManager;
			static TransformSystem* m_transformSystem;
			static RenderSystem* m_meshRendererSystem;
			static PhysicsSystem* m_physicsSystem;
		};
	}
}

#endif