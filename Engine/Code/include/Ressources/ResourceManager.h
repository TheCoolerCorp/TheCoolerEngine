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

			ENGINE_API void CreateResource(ResourceType a_type, std::string a_path, std::string a_name);
			ENGINE_API void DestroyResource(std::string a_name);

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