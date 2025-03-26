#ifndef COMPONENTSPOOL_H
#define COMPONENTSPOOL_H

#include "EngineExport.h"

#include "Core/Logger/Logger.h"
#include "GamePlay/Components/TransformComponent.h"
#include "GamePlay/Components/Meshcomponent.h"

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
				// Check if Type is a derived component and not the base class.
				static_assert(std::is_base_of<Component, Type>::value);
				static_assert(!std::is_same<Component, Type>::value);

				// Add the component to the right place holder.
				if (std::is_same<Type, TransformComponent>::value/* && typeid(*a_component) == typeid(TransformComponent)*/)
				{
					m_transformComponents.emplace(a_id, dynamic_cast<TransformComponent*>(a_component));
				}
				else if (std::is_same<Type, TransformComponent>::value/* && typeid(*a_component) == typeid(MeshComponent)*/)
				{
					m_meshesComponents.emplace(a_id, dynamic_cast<MeshComponent*>(a_component));
				}
				else
				{
					LOG_ERROR("This type of component");
				}
			}

			template<typename Type>
			Type* GetComponent(int a_id)
			{
				// Check if Type is a derived component and not the base class.
				static_assert(std::is_base_of<Component, Type>::value);
				static_assert(!std::is_same<Component, Type>::value);

				// Find the component in the right place holder.
				if (std::is_same<Type, TransformComponent>::value)
				{
					auto it = m_transformComponents.find(a_id);

					if (it != m_transformComponents.end()) 
					{
						return dynamic_cast<Type*>(it->second);
					}
				}
				else if (std::is_same<Type, MeshComponent>::value)
				{
					auto it = m_meshesComponents.find(a_id);

					if (it != m_meshesComponents.end()) 
					{
						return dynamic_cast<Type*>(it->second);
					}
				}
				else
				{
					LOG_ERROR("Unsupported component type requested");
				}

				return nullptr;
			}

			template<typename Type>
			void RemoveComponent(int a_id)
			{	
				// Check if Type is a derived component and not the base class.
				static_assert(std::is_base_of<Component, Type>::value);
				static_assert(!std::is_same<Component, Type>::value);

				// Find the right place holder and then Destroy the component and delete the pointer.
				if (std::is_same<Type, TransformComponent>::value)
				{
					if (m_transformComponents.find(a_id))
					{
						m_transformComponents.at(a_id)->Destroy();
						delete m_transformComponents.at(a_id);
					}
				}
				else if (std::is_same<Type, MeshComponent>::value)
				{
					if (m_meshesComponents.find(a_id))
					{
						m_meshesComponents.at(a_id)->Destroy();
						delete m_transformComponents.at(a_id);
					}
				}
				else
				{
					LOG_ERROR("Unsupported component type requested");
				}
			}

			std::vector<int>& GetIds() { return m_registerId; }

		private:
			std::unordered_map<int, TransformComponent*> m_transformComponents;
			std::unordered_map<int, MeshComponent*> m_meshesComponents;

			std::vector<int> m_registerId;

			// TODO
			//std::unordered_map<int, RHI::IDescriptorSets*> m_meshesComponents;
		};
	}
}
#endif