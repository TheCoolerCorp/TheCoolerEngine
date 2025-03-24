#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "EngineExport.h"
#include  "Core/Logger/Logger.h"

#include <unordered_map>
#include <string>

#include "IResource.h"
#include "Mesh.h"
#include "Texture.h"
#include "Core/Utils.h"

namespace Engine
{
	namespace Resource
	{
		class ResourceManager
		{
		public:
			template<typename Type, typename... Args>
			std::shared_ptr<Type> CreateResource(Args&&... args)
			{
				static_assert(std::is_base_of<IResource, Type>::value);
				static_assert(std::is_member_function_pointer_v<decltype(&Type::Create)>);

				std::shared_ptr<Type> t_resource = std::make_shared<Type>();
				t_resource->Create(std::forward<Args>(args)...);

				for (const auto& [id, resource] : m_resources)
				{
					if (resource == t_resource)
					{
						return std::static_pointer_cast<Type>(resource);
					}
				}
				int t_randomId = Utils::GenerateRandomInt(0, INT16_MAX);
				while (m_resources.find(t_randomId) != m_resources.end())
				{
					t_randomId = Utils::GenerateRandomInt(0, INT16_MAX);
				}
				m_resources.at(t_randomId) = t_resource;

				return t_resource;

			}
			//ENGINE_API IResource* CreateResourceReference(ResourceType a_type, std::string a_path, std::string a_name, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool);
			//ENGINE_API void DestroyResource(std::string a_name, Core::RHI::ILogicalDevice* a_logicalDevice);

			//ENGINE_API IResource* GetResource(std::string a_name);

		private:
			std::unordered_map<int, std::shared_ptr<IResource>> m_resources;
		};

	}
}

#endif 