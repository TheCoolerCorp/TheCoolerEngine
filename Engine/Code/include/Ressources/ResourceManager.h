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
			Ref<Type> CreateResource(const std::string& a_path, Args&&... a_args);

			template<typename Type, typename... Args>
			Ref<Type> GetResource(const std::string& a_path, Args&&... a_args);

			ENGINE_API void DestroyAll(Core::Renderer* a_renderer);

		private:
			std::unordered_map<std::string, Ref<IResource>> m_resources;
		};
	}
}

#include "ResourceManager.inl"

#endif 