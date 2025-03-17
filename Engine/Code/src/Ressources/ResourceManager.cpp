#include  "Ressources/ResourceManager.h"

namespace Engine
{
	namespace Resource
	{
		ResourceManager& ResourceManager::Get()
		{
			static ResourceManager m_instance;
			return m_instance;
		}
	}
}