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

				std::string t_path = NormalizePath(a_path);

				if (m_resources.contains(t_path))
				{
					return std::dynamic_pointer_cast<Type>(m_resources.at(t_path));
				}

				Ref<Type> t_resource = CreateRef<Type>();
				t_resource->Create(t_path);

				m_resources.emplace(t_path, t_resource);

				return t_resource;
			}
		}

		template <typename Type, typename ... Args>
		Ref<Type> ResourceManager::GetResource(const std::string& a_path, Args&&... a_args)
		{
			std::string t_path = NormalizePath(a_path);

			if (!m_resources.contains(t_path))
			{
				LOG_ERROR("No existing resource at path : " + t_path);
				return nullptr;
			}

			return std::dynamic_pointer_cast<Type>(m_resources.at(t_path));
		}

		inline std::string ResourceManager::NormalizePath(const std::string& a_path)
		{
			std::string t_result = a_path;
			std::ranges::replace(t_result, '/', '\\');
			return t_result;
		}
	}
}
#endif // RESOURCEMANAGER_INL
