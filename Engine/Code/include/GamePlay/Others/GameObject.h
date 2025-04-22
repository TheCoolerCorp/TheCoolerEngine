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
				ENGINE_API GameObject(std::string a_name = "Game Object");
				ENGINE_API GameObject(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name = "Game Object");

				ENGINE_API ~GameObject() = default;

				void Create(Core::Renderer* a_renderer);
				/*void Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info);
				void Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice);
				void Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice);
				GameObjectData SubmitData();*/


				template<typename ComponentClass>
				void AddComponent()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_member_function_pointer<decltype(&ComponentClass::Create)>::value);

					ComponentClass* t_newComponent = new ComponentClass();

					uint32_t id = -1;
					ComponentType t_componentType = t_newComponent->Create(id);

					m_compsId.insert({ t_componentType, id });
				}

				template<typename ComponentClass>
				ComponentClass* GetComponent()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetType)>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetComponent), uint32_t>::value);
						
					ComponentType t_componentType = ComponentClass::GetType();
					uint32_t t_id = m_compsId.at(t_componentType);

					return ComponentClass::GetComponent(t_id);
				}

				template<typename ComponentClass>
				uint32_t GetComponentID()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetType)>::value);

					ComponentType t_componentType = ComponentClass::GetType();

					return m_compsId.at(t_componentType);
				}

				template<typename ComponentClass>
				void RemoveComponent()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetType)>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::RemoveComponent), uint32_t>::value);

					ComponentType t_componentType = ComponentClass::GetType();
					uint32_t t_id = m_compsId.at(t_componentType);

					ComponentClass::RemoveComponent(t_id);
				}

				void SetName(const std::string& a_name) { m_name = a_name; }
				void SetParent(uint32_t a_id);
				void AddChild(uint32_t a_id);
				void RemoveChild(uint32_t a_id);
				void ClearChildren();

				[[nodiscard]] uint32_t GetParentId() const { return m_parentId; }
				[[nodiscard]] std::vector<uint32_t> GetChildrenIDs() const { return m_childrenIds; }
				[[nodiscard]] std::string GetName() const { return m_name; }

			private:
				std::unordered_map<ComponentType, uint32_t> m_compsId = std::unordered_map<ComponentType, uint32_t>(0);

				std::string m_name;
				//id of the parent gameObject
				uint32_t m_parentId = -1;
				//id of the child gameObjects
				std::vector<uint32_t> m_childrenIds{};

			};

		
	}
}

#endif 