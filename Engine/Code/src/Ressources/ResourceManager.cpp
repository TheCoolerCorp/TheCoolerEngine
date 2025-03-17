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

		void ResourceManager::CreateResource(ResourceType a_type, std::string a_path, std::string& a_name)
		{
			if (a_type == ResourceType::MESH)
			{
				Mesh* t_mesh = new Mesh();
				t_mesh->Create(a_path);
				m_resources[a_name] = t_mesh;
			}
			else if (a_type == ResourceType::TEXTURE)
			{
				Texture* t_texture = new Texture();
				t_texture->Create(a_path);
				m_resources[a_name] = t_texture;
			}
			else
			{
				LOG_ERROR("Type of resource not implemented !");
			}
		}

		void ResourceManager::DestroyResource(std::string a_name)
		{
			auto it = m_resources.find(a_name);
			if (it != m_resources.end())
			{
				delete it->second;
				m_resources.erase(it);
			}
			LOG_WARNING("[Resource Manager] Can't find this item : " + a_name);
			return;
		}

		IResource* ResourceManager::GetResource(std::string a_name)
		{
			auto it = m_resources.find(a_name);
			if (it != m_resources.end())
			{
				return it->second;
			}
			LOG_WARNING("[Resource Manager] Can't find this item : " + a_name);
			return nullptr;
		}
	}
}