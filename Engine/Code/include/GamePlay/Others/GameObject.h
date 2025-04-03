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
			ENGINE_API GameObject();
			ENGINE_API GameObject(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale);

			ENGINE_API ~GameObject() = default;

			void Create(Core::Renderer* a_renderer);
			/*void Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info);
			void Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice);
			void Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice);
			GameObjectData SubmitData();*/


			template<typename ComponentClass>
			void AddComponent()
			{
				static_assert(std::is_base_of_v<ComponentClass, Component>);
				static_assert(!std::is_same_v<ComponentClass, Component>);
				static_assert(std::is_member_function_pointer<decltype(&ComponentClass::Create)>::value);

				ComponentClass* t_newComponent = new ComponentClass();

				uint32_t id = 0;
				ComponentType t_componentType = t_newComponent->Create(id);

				m_compsId.insert({ t_componentType, id });
			}

			template<typename ComponentClass>
			ComponentClass* GetComponent()
			{
				static_assert(std::is_base_of_v<ComponentClass, Component>);
				static_assert(!std::is_same_v<ComponentClass, Component>);
				static_assert(std::is_member_function_pointer<decltype(&ComponentClass::GetType)>::value);
				static_assert(std::is_member_function_pointer<decltype(&ComponentClass::GetComponent)>::value);

				ComponentType t_componentType = ComponentClass::GetType();
				uint32_t t_id = m_compsId.at(t_componentType);

				return ComponentClass::GetComponent(t_id);
			}

			template<typename ComponentClass>
			void RemoveComponent()
			{
				static_assert(std::is_base_of_v<ComponentClass, Component>);
				static_assert(!std::is_same_v<ComponentClass, Component>);
				static_assert(std::is_member_function_pointer<decltype(&ComponentClass::GetType)>::value);
				static_assert(std::is_member_function_pointer<decltype(&ComponentClass::RemoveComponent)>::value);

				ComponentType t_componentType = ComponentClass::GetType();
				uint32_t t_id = m_compsId.at(t_componentType);

				ComponentClass::RemoveComponent(t_id);
			}

		private:
			std::unordered_map<ComponentType, uint32_t> m_compsId;
		};

		
	}
}

#endif 