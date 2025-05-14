#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "EngineExport.h"

#include "Ressources/ResourceManager.h"
#include "GamePlay/Systems/TransformSystem.h"
#include "GamePlay/Systems/RenderSystem.h"
#include "GamePlay/Systems/PhysicsSystem.h"
#include "GamePlay/Systems/GameComponentSystem.h"

namespace Engine::Core::Multithread {
	class ThreadPool;
}

namespace Engine
{
	namespace GamePlay
	{
		class ServiceLocator
		{
		public:
			ENGINE_API static void ProvideThreadPool(Core::Multithread::ThreadPool* a_threadPool);
			ENGINE_API static void ProvideResourceManager(Resource::ResourceManager* a_resourceManager);
			ENGINE_API static void ProvideTransformSystem(TransformSystem* a_transformSystem);
			ENGINE_API static void ProvideRenderSystem(RenderSystem* a_meshRendererSystem);
			ENGINE_API static void ProvidePhysicsSystem(PhysicsSystem* a_physicsSystem);
			ENGINE_API static void ProvideRenderer(Core::Renderer* a_renderer);
			ENGINE_API static void ProvideGameComponentSystem(GameComponentSystem* a_gameComponentSystem);

			ENGINE_API static Core::Multithread::ThreadPool* GetThreadPool();
			ENGINE_API static Resource::ResourceManager* GetResourceManager();
			ENGINE_API static TransformSystem* GetTransformSystem();
			ENGINE_API static RenderSystem* GetRendererSystem();
			ENGINE_API static PhysicsSystem* GetPhysicsSystem();
			ENGINE_API static Core::Renderer* GetRenderer();
			ENGINE_API static GameComponentSystem* GetGameComponentSystem();

		private:
			static Core::Multithread::ThreadPool* m_threadPool;
			static Resource::ResourceManager* m_resourceManager;
			static TransformSystem* m_transformSystem;
			static RenderSystem* m_meshRendererSystem;
			static PhysicsSystem* m_physicsSystem;
			static GameComponentSystem* m_gameComponentSystem;
			static Core::Renderer* m_renderer;
		};
	}
}

#endif