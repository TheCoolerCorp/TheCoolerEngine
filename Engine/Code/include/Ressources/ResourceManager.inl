#ifndef RESOURCEMANAGER_INL
#define RESOURCEMANAGER_INL

namespace Engine
{
	namespace Resource
	{
		template <typename Type, typename ... Args>
		Ref<Type> ResourceManager::CreateResource(const std::string& a_path, Args&&... a_args)
		{
			{
				static_assert(std::is_base_of<IResource, Type>::value);
				static_assert(!std::is_same<IResource, Type>::value);
				static_assert(std::is_member_function_pointer<decltype(&Type::Create)>::value);

				if (m_resources.contains(a_path))
				{
					return m_resources.at(a_path);
				}

				Ref<Type> t_resource = CreateRef<Type>();
				t_resource->Create(a_path);

				m_resources.emplace(a_path, t_resource);

				return t_resource;
			}
		}

		template <typename Type, typename ... Args>
		Ref<Type> ResourceManager::GetResource(const std::string& a_path, Args&&... a_args)
		{
			if (!m_resources.contains(a_path))
			{
				LOG_ERROR("No existing resource at path : " + a_path);
				return nullptr;
			}

			return m_resources.at(a_path);
		}
	}
}
#endif // RESOURCEMANAGER_INL
