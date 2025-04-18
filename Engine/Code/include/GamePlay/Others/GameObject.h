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
				ENGINE_API GameObject();
				ENGINE_API GameObject(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale);

				ENGINE_API ~GameObject() = default;

				void Create(Core::Renderer* a_renderer);
				ENGINE_API [[nodiscard]] Math::mat4 GetColliderMat() const { return m_colliderMat; }
				ENGINE_API void UpdateColliderMat();
				/*void Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info);
				void Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice);
				void Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice);
				GameObjectData SubmitData();*/


				template<typename ComponentClass>
				void AddComponent(bool a_colliderMesh = false)
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_member_function_pointer<decltype(&ComponentClass::Create)>::value);

					ComponentClass* t_newComponent = new ComponentClass();

					uint32_t id = -1;
					ComponentType t_componentType = t_newComponent->Create(id, a_colliderMesh);

					if (m_compsId.contains(t_componentType))
					{
						return;
					}

					m_compsId.insert({ t_componentType, id });
				}

				template<typename ComponentClass>
				ComponentClass* GetComponent(bool a_colliderMesh = false)
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetType), bool>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetComponent), uint32_t>::value);
						
					ComponentType t_componentType = ComponentClass::GetType(a_colliderMesh);

					if (!m_compsId.contains(t_componentType))
					{
						return nullptr;
					}

					uint32_t t_id = m_compsId.at(t_componentType);

					return ComponentClass::GetComponent(t_id);
				}

				template<typename ComponentClass>
				int GetComponentID(bool a_colliderMesh = false)
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetType), bool>::value);

					ComponentType t_componentType = ComponentClass::GetType(a_colliderMesh);

					if (!m_compsId.contains(t_componentType))
					{
						return -1;
					}

					return m_compsId.at(t_componentType);
				}

				template<typename ComponentClass>
				void RemoveComponent(bool a_colliderMesh = false)
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetType)>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::RemoveComponent), uint32_t>::value);

					ComponentType t_componentType = ComponentClass::GetType(a_colliderMesh);

					if (!m_compsId.contains(t_componentType))
					{
						return;
					}

					uint32_t t_id = m_compsId.at(t_componentType);

					ComponentClass::RemoveComponent(t_id);
				}

			private:
				std::unordered_map<ComponentType, uint32_t> m_compsId = std::unordered_map<ComponentType, uint32_t>(0);

				Math::mat4 m_colliderMat;
			};

		
	}
}

#endif 