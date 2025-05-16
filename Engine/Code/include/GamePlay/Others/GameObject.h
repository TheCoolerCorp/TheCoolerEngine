#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <bitset>
#include <map>

#include "EngineExport.h"

#include <vector>
#include <string>


#include "GamePlay/Components/Component.h"

//#include "Gameplay/ServiceLocator.h"
#include "Math/vec3.h"
#include "Math/mat4.h"
#include <typeindex>

#include <unordered_map>

#include "Core/Window/IInputHandler.h"

namespace Engine
{
	namespace GamePlay
	{
			class GameObject
			{
			public:
				ENGINE_API GameObject( std::string a_name = "Game Object");
				ENGINE_API GameObject( Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name = "Game Object");
				ENGINE_API GameObject( Math::vec3 a_position, Math::quat a_rotation, Math::vec3 a_scale, std::string a_name = "Game Object");

				ENGINE_API virtual ~GameObject();

				//ENGINE_API void Create(Core::Renderer* a_renderer);
				ENGINE_API [[nodiscard]] Math::mat4 GetColliderMat() { m_colliderMat.Transpose(); return m_colliderMat; }
				ENGINE_API void UpdateColliderMat();

				template<typename ComponentClass>
				void AddComponent()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_member_function_pointer<decltype(&ComponentClass::Create)>::value);

					ComponentClass* t_newComponent = new ComponentClass();
					t_newComponent->SetGameObject(m_id);
					int id = -1;

					t_newComponent->Create(id);
					const std::type_info& t_componentIdentifier = Component::GetCompId<ComponentClass>();

					if (m_compsId.contains(t_componentIdentifier))
					{
						return;
					}

					m_compsId.insert({ t_componentIdentifier, id });
				}

				template<typename ComponentClass>
				ComponentClass* GetComponent()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::GetComponent), uint32_t>::value);
						
					const std::type_info& t_componentIdentifier = Component::GetCompId<ComponentClass>();

					if (!m_compsId.contains(t_componentIdentifier))
					{
						return nullptr;
					}

					int t_id = m_compsId.at(t_componentIdentifier);

					return ComponentClass::GetComponent(t_id);
				}


				template<typename ComponentClass>
				bool HasComponent()
				{
					const std::type_info& t_componentIdentifier = Component::GetCompId<ComponentClass>();

					if (!m_compsId.contains(t_componentIdentifier))
					{
						return false;
					}

					return true;
				}

				template<typename ComponentClass>
				int GetComponentID()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);

					const std::type_info& t_componentIdentifier = Component::GetCompId<ComponentClass>();

					if (!m_compsId.contains(t_componentIdentifier))
					{
						return -1;
					}

					return m_compsId.at(t_componentIdentifier);
				}

				template<typename ComponentClass>
				void RemoveComponent()
				{
					static_assert(std::is_base_of<Component, ComponentClass>::value);
					static_assert(!std::is_same<ComponentClass, Component>::value);
					static_assert(std::is_invocable<decltype(&ComponentClass::RemoveComponent), int>::value);

					const std::type_info& t_componentIdentifier = Component::GetCompId<ComponentClass>();

					if (!m_compsId.contains(t_componentIdentifier))
					{
						return;
					}

					int t_id = m_compsId.at(t_componentIdentifier);

					ComponentClass::RemoveComponent(t_id);
					m_compsId.erase(t_componentIdentifier);
				}

				ENGINE_API void SceneUpdate();
				ENGINE_API void Update();
				ENGINE_API void ProcessInputs(Core::Window::IInputHandler* a_inputHandler, float a_deltaTime);

				ENGINE_API void SetId(int a_id);
				ENGINE_API void SetName(const std::string& a_name) { m_name = a_name; }
				ENGINE_API void SetParent(int a_transformId);
				ENGINE_API void RemoveParent();
				ENGINE_API void AddChild(int a_transformId);
				ENGINE_API void RemoveChild(int a_transformId);
				ENGINE_API void ClearChildren();

				[[nodiscard]] ENGINE_API int GetParentTransformId();
				[[nodiscard]] ENGINE_API std::vector<int> GetChildrenTransformIDs();
				[[nodiscard]] ENGINE_API std::string GetName() const { return m_name; }
				[[nodiscard]] ENGINE_API int GetId() const { return m_id; }
				[[nodiscard]] ENGINE_API std::vector<std::type_index> GetOwnedTypes();
					
				ENGINE_API bool HasParent();
				ENGINE_API bool HasChildren();
			protected:
				//the first int is the gameobjects Unique identifier. There is one for each class type that remains unique for the runtime
				std::unordered_map<std::type_index, int> m_compsId = std::unordered_map<std::type_index, int>(0);


				Math::mat4 m_colliderMat;

				std::string m_name;
				int m_id = -1;

				void ClearComponents();
			};

		
	}
}

#endif 