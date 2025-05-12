#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		Core::Multithread::ThreadPool* ServiceLocator::m_threadPool = nullptr;
		Resource::ResourceManager* ServiceLocator::m_resourceManager = nullptr;
		TransformSystem* ServiceLocator::m_transformSystem = nullptr;
		RenderSystem* ServiceLocator::m_meshRendererSystem = nullptr;
		PhysicsSystem* ServiceLocator::m_physicsSystem = nullptr;
		Core::Renderer* ServiceLocator::m_renderer = nullptr;

		void ServiceLocator::ProvideThreadPool(Core::Multithread::ThreadPool* a_threadPool)
		{
			m_threadPool = a_threadPool;
		}

		void ServiceLocator::ProvideResourceManager(Resource::ResourceManager* a_resourceManager)
		{
			m_resourceManager = a_resourceManager;
		}

		void ServiceLocator::ProvideTransformSystem(TransformSystem* a_transformSystem)
		{
			m_transformSystem = a_transformSystem;
		}

		void ServiceLocator::ProvideRenderSystem(RenderSystem* a_meshRendererSystem)
		{
			m_meshRendererSystem = a_meshRendererSystem;
		}

		void ServiceLocator::ProvidePhysicsSystem(PhysicsSystem* a_physicsSystem)
		{
			m_physicsSystem = a_physicsSystem;
		}

		void ServiceLocator::ProvideRenderer(Core::Renderer* a_renderer)
		{
			m_renderer = a_renderer;
		}

		Core::Multithread::ThreadPool* ServiceLocator::GetThreadPool()
		{
			return m_threadPool;
		}

		Resource::ResourceManager* ServiceLocator::GetResourceManager()
		{
			return m_resourceManager;
		}

		TransformSystem* ServiceLocator::GetTransformSystem()
		{
			return m_transformSystem;
		}

		RenderSystem* ServiceLocator::GetRendererSystem()
		{
			return m_meshRendererSystem;
		}

		PhysicsSystem* ServiceLocator::GetPhysicsSystem()
		{
			return m_physicsSystem;
		}

		Core::Renderer* ServiceLocator::GetRenderer()
		{
			return m_renderer;
		}
	}
}