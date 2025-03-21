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

		ResourceManager::~ResourceManager()
		{
			if (m_resources.empty())
			{
				return;
			}

			// NEED CHANGE HERE
			/*for (auto resource : m_resources)
			{
				if (resource.second)
				{
					delete resource.second;
				}
				m_resources.erase(resource.first);
			}*/
		}


		void ResourceManager::CreateResource(ResourceType a_type, std::string a_path, std::string a_name, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool)
		{
			if (a_type == ResourceType::MESH)
			{
				Mesh* t_mesh = new Mesh();
				t_mesh->Create(a_path, a_interface, a_physicalDevice, a_logicalDevice, a_commandPool);
				m_resources[a_name] = t_mesh;
			}
			else if (a_type == ResourceType::TEXTURE)
			{
				Texture* t_texture = new Texture();
				t_texture->Create(a_path, a_interface, a_physicalDevice, a_logicalDevice, a_commandPool);
				m_resources[a_name] = t_texture;
			}
			else
			{
				LOG_ERROR("Type of resource not implemented !");
			}
		}

		IResource* ResourceManager::CreateResourceReference(ResourceType a_type, std::string a_path, std::string a_name, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool)
		{
			if (a_type == ResourceType::MESH)
			{
				Mesh* t_mesh = new Mesh();
				t_mesh->Create(a_path, a_interface, a_physicalDevice, a_logicalDevice, a_commandPool);
				m_resources[a_name] = t_mesh;
				return t_mesh;
			}
			else if (a_type == ResourceType::TEXTURE)
			{
				Texture* t_texture = new Texture();
				t_texture->Create(a_path, a_interface, a_physicalDevice, a_logicalDevice, a_commandPool);
				m_resources[a_name] = t_texture;
				return t_texture;
			}
			else
			{
				LOG_ERROR("Type of resource not implemented !");
				return nullptr;
			}
		}

		void ResourceManager::DestroyResource(std::string a_name, Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			auto it = m_resources.find(a_name);
			if (it != m_resources.end())
			{
				it->second->Destroy(a_logicalDevice);
				delete it->second;
				m_resources.erase(it);
				return;
			}
			LOG_WARNING("Can't find this item : " + a_name);
		}

		IResource* ResourceManager::GetResource(std::string a_name)
		{
			auto it = m_resources.find(a_name);
			if (it != m_resources.end())
			{
				return it->second;
			}
			LOG_WARNING("Can't find this item : " + a_name);
			return nullptr;
		}
	}
}