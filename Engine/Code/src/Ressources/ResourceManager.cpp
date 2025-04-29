#include  "Ressources/ResourceManager.h"

namespace Engine
{
	namespace Resource
	{
		void ResourceManager::DestroyAll(Core::Renderer* a_renderer)
		{
			for (auto& [id, resource] : m_resources) 
			{
				resource->Unload(a_renderer);
				resource->Destroy();
				resource.reset();
			}
			m_resources.clear();
		}
	}
}