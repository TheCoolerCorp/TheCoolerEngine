#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <ranges>

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
			Ref<Type> CreateResource(Args&&... args)
			{
				static_assert(std::is_base_of<IResource, Type>::value);
				static_assert(!std::is_same<IResource, Type>::value);
				static_assert(std::is_member_function_pointer<decltype(&Type::Create)>::value);

				Ref<Type> t_resource = CreateRef<Type>();
				t_resource->Create(std::forward<Args>(args)...);

				for (auto& t_toFindResource : m_resources | std::views::values)
				{
					if (t_toFindResource == t_resource)
					{
						return t_resource;
					}
				}

				int t_id = -1;
				const int t_resourcesSize = static_cast<int>(m_resources.size());

				if (t_resourcesSize != 0)
				{
					t_id = t_resourcesSize;
				}
				else
				{
					t_id = 0;
				}

				m_resources.emplace(t_id, t_resource);

				return t_resource;
			}

			ENGINE_API void DestroyAll(Core::Renderer* a_renderer);

		private:
			std::unordered_map<int, Ref<IResource>> m_resources;
		};

	}
}

#endif 