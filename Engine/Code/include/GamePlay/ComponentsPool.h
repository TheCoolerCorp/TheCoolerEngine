#ifndef COMPONENTSPOOL_H
#define COMPONENTSPOOL_H

#include "EngineExport.h"

#include "Core/Logger/Logger.h"
#include "GamePlay/TransformComponent.h"
#include "GamePlay/Meshcomponent.h"

#include <unordered_map>


namespace Engine
{
	namespace GamePlay
	{
		class ComponentsPool
		{
		public:
			~ComponentsPool() = default;

			ENGINE_API void Init();
			ENGINE_API void Destroy();

			ENGINE_API void RegisterTransform(int a_id, TransformComponent* a_transformComponent);
			ENGINE_API void RegisterMesh(int a_id, MeshComponent* a_meshComponent);

			template<typename Type>
			void RegisterComponent(Type* a_component, int a_id)
			{
				const char* t_componentType = typeid(a_component).name();

				if (typeid(TransformComponent*).name() == t_componentType)
				{
					m_transformComponents.emplace(a_id, a_component);
				}
				else if (typeid(MeshComponent*).name() == t_componentType)
				{
					m_meshesComponents.emplace(a_id, a_component);
				}
				else
				{
					LOG_ERROR("This type of component");
				}
			}

			template<typename Type>
			Type* GetComponent(int a_id)
			{
				const char* t_componentType = typeid(Type*).name();

				if (typeid(TransformComponent*).name() == t_componentType)
				{
					if (m_transformComponents.find(a_id))
					{
						return m_transformComponents.at(a_id);
					}
					return nullptr;
				}
				else if (typeid(MeshComponent*).name() == t_componentType)
				{
					if (m_meshesComponents.find(a_id))
					{
						return m_meshesComponents.at(a_id);
					}
					return nullptr;
				}
				else
				{
					LOG_ERROR("This type of component");
				}
			}

			template<typename Type>
			void RemoveComponent(int a_id)
			{	
				const char* t_componentType = typeid(Type).name();

				if (typeid(TransformComponent*).name() == t_componentType)
				{
					m_transformComponents.at(a_id)->Destroy();
					m_transformComponents.at(a_id) = nullptr;
				}
				else if (typeid(MeshComponent*).name() == t_componentType)
				{
					m_meshesComponents.at(a_id)->Destroy();
					m_meshesComponents.at(a_id) = nullptr;
				}
				else
				{
					LOG_ERROR("This type of component");
				}
			}

		private:
			std::unordered_map<int, TransformComponent*> m_transformComponents;
			std::unordered_map<int, MeshComponent*> m_meshesComponents;

			std::vector<int> m_registerId;
		};
	}
}
#endif