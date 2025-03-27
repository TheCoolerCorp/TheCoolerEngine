#include "GamePlay/Others/Scene.h"

namespace Engine
{
	namespace GamePlay
	{
		void Scene::Create(Core::Renderer& a_renderer)
		{
			m_componentsPool = new ComponentsPool;
			m_transformSystem = new TransformSystem;
			m_renderSystem = new RenderSystem;
			m_resourceManager = new Resource::ResourceManager;
		}

		void Scene::Update(Core::Renderer& a_renderer)
		{
			m_transformSystem->Update(*m_componentsPool);
			m_renderSystem->Update(*m_componentsPool, a_renderer);
		}

		void Scene::Render(Core::Renderer& a_renderer)
		{
			
		}

		void Scene::Destroy(Core::Renderer& a_renderer)
		{
			m_renderSystem->End(*m_componentsPool, a_renderer);
			delete m_renderSystem;

			delete m_transformSystem;

			m_componentsPool->Destroy();

			
		}
	}
}