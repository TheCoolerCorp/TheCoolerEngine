#include  "Ressources/ResourceManager.h"

namespace Engine
{
	namespace Resource
	{
		void ResourceManager::DestroyAll(Core::Renderer* a_renderer)
		{
			for (auto& [id, resource] : m_resources) 
			{
				
				resource->Destroy();
				delete resource;
			}
		}
	}
}