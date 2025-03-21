#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "EngineExport.h"
#include  "Core/Logger/Logger.h"

#include <unordered_map>
#include <string>

#include "IResource.h"
#include "Mesh.h"
#include "Texture.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class ApiInterface;
		}
	}

	namespace Resource
	{
		enum class ResourceType
		{
			UNDEFINED = 0,
			MESH = 1,
			TEXTURE = 2
		};


		class ResourceManager
		{
		public:
			ENGINE_API static ResourceManager& Get();

			ENGINE_API void CreateResource(ResourceType a_type, std::string a_path, std::string a_name, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool);
			ENGINE_API IResource* CreateResourceReference(ResourceType a_type, std::string a_path, std::string a_name, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool);
			ENGINE_API void DestroyResource(std::string a_name, Core::RHI::ILogicalDevice* a_logicalDevice);

			ENGINE_API IResource* GetResource(std::string a_name);

		private:
			ResourceManager() = default;
			~ResourceManager();
			ResourceManager(const ResourceManager&) = delete;
			ResourceManager& operator=(const ResourceManager&) = delete;

			std::unordered_map<std::string, IResource*> m_resources;
		};

	}
}

#endif 