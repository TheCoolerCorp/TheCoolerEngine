#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <bitset>

#include "EngineExport.h"

#include <vector>
#include <string>

#include "Core/Logger/Logger.h"

#include "Math/Transform.h"
#include "GamePlay/Components/Component.h"
#include "Ressources/ResourceManager.h"
#include "Gameplay/Components/ComponentsPool.h"


#include "Core/Interfaces/IObjectDescriptor.h"
#include "Core/Interfaces/ApiInterface.h"

#include "Gameplay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		class GameObject
		{
		public:
			GameObject();
			GameObject(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale);

			~GameObject() = default;


			/*void Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info);
			void Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice);
			void Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice);
			GameObjectData SubmitData();*/


			template<typename Type>
			void AddComponent()
			{
				// Check if Type is a derived component and not the base class.
				static_assert(std::is_base_of<Component, Type>::value);
				static_assert(!std::is_same<Component, Type>::value);

				Type* t_component = new Type();
				//t_component->Create():
				
				// Add this component to the pool.
				ServiceLocator::GetComponentsPool()->RegisterComponent<Type>(t_component, m_id);
			}

			template<typename Type>
			Type* GetComponent()
			{
				// Check if Type is a derived component and not the base class.
				static_assert(std::is_base_of<Component, Type>::value);
				static_assert(!std::is_same<Component, Type>::value);

				//return ServiceLocator::GetComponentsPool()->GetComponent<Type>(m_id);
				return nullptr;
			}

			template<typename Type>
			void RemoveComponent()
			{
				// Check if Type is a derived component and not base class.
				static_assert(std::is_base_of<Component, Type>::value);
				static_assert(!std::is_same<Component, Type>::value);

				//Type* t_component = new Type();
				//ServiceLocator::GetComponentsPool()->RegisterComponents<Type>(t_component, m_id);
			}
			//template<typename Type, typename... Args>
			//void AddComponent(Args&&... args)
			//{
			//	static_assert(std::is_base_of<Component, Type>::value);
			//	//static_assert(std::is_member_function_pointer<decltype(&Type::Create)>::value);

			//	if (HasComponent<Type>())
			//	{
			//		// TODO : rework Debug message later
			//		//std::string warning_error = "GameObject :" + std::to_string(m_id) + "already have this type of component" + std::to_string(typeid(Type));
			//		LOG_WARNING("Error");
			//		return;
			//	}

			//	std::shared_ptr<Type> t_component = std::make_shared<Type>();
			//	t_component->Create(std::forward<Args>(args)...);
			//	ServiceLocator::GetComponentsPool()->RegisterComponent(t_component, m_id);
			//}

			//template<typename Type>
			//std::shared_ptr<Type> GetComponent(ComponentsPool& a_componentsPool)
			//{
			//	static_assert(std::is_base_of<Component, Type>::value);
			//	//for (const auto& component : m_components)
			//	//{
			//	//	std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component);

			//	//	if (castedComponent)
			//	//	{
			//	//		return castedComponent;
			//	//	}
			//	//}
			//	//return nullptr;
			//}

			//template<typename Type>
			//bool HasComponent()
			//{
			//	static_assert(std::is_base_of<Component, Type>::value);



			//	/*for (auto it = m_components.begin(); it != m_components.end();)
			//	{
			//		std::shared_ptr<T> component = dynamic_cast<std::shared_ptr<T>()>(it);
			//		if (component)
			//		{
			//			component->Destroy();
			//			it = m_components.erase(it);
			//		}
			//		else
			//		{
			//			++it;
			//		}
			//	}*/
			//	return true;
			//}


		private:
			Core::RHI::IObjectDescriptor* m_descriptor{};
			Core::RHI::IDescriptorPool* m_descriptorPool{};

			static std::bitset<INT32_MAX> m_idBitset;

			int m_id = 0;
		};

		
	}
}

#endif 