#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <bitset>
#include <map>

#include "EngineExport.h"

#include <vector>
#include <string>

#include "Core/Logger/Logger.h"

#include "GamePlay/Components/Component.h"

#include "Math/Transform.h"
#include "GamePlay/Components/Component.h"
#include "Ressources/ResourceManager.h"
#include "Gameplay/Components/ComponentsPool.h"


#include "Core/Interfaces/IObjectDescriptor.h"
#include "Core/Interfaces/ApiInterface.h"

#include "Gameplay/ServiceLocator.h"
#include "Core/Renderer/Renderer.h"

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

			void Create(Core::Renderer* a_renderer);
			/*void Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info);
			void Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice);
			void Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice);
			GameObjectData SubmitData();*/


			template<typename ComponentClass>
			void AddComponent()
			{
				static_assert(std::is_base_of<ComponentClass, Component>::value);
				static_assert(!std::is_same<ComponentClass, Component>::value);
				static_assert(std::is_member_function_pointer<decltype(&ComponentClass::Create)>::value);

				ComponentClass* t_newComponent = new ComponentClass();

				uint32_t id = 0;
				ComponentType t_componentType = t_newComponent->Create(&id);

				m_compsId.insert({ id, t_componentType });
			}

		//	template<typename Type>
		//	Type* GetComponent()
		//	{
		//		// Check if Type is a derived component and not the base class.
		//		static_assert(std::is_base_of<Component, Type>::value);
		//		static_assert(!std::is_same<Component, Type>::value);

		//		// Get the component from the pool.
		//		return ServiceLocator::GetComponentsPool()->GetComponent<Type>(m_id);
		//	}

		//	template<typename Type>
		//	void RemoveComponent()
		//	{
		//		// Check if Type is a derived component and not base class.
		//		static_assert(std::is_base_of<Component, Type>::value);
		//		static_assert(!std::is_same<Component, Type>::value);

		//		//Type* t_component = new Type();
		//		ServiceLocator::GetComponentsPool()->RemoveComponent<Type>(m_id);
		//	}
		//	
		//private:
		//	Core::RHI::IObjectDescriptor* m_descriptor{};
		//	Core::RHI::IDescriptorPool* m_descriptorPool{};

		//	static std::bitset<INT32_MAX> m_idBitset;

			//uint32_t m_meshId = -1;

			// vector<id>


			std::unordered_map<uint32_t, ComponentType> m_compsId;
			
		};

		
	}
}

#endif 