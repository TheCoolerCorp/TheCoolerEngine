#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "EngineExport.h"

#include <vector>
#include <string>

#include "Core/Logger/Logger.h"
#include "Component.h"
#include "Math/Transform.h"

namespace Engine
{
	namespace GamePlay
	{
		class GameObject
		{
		public:
			GameObject() = default;
			~GameObject() = default;

			GameObject(Math::vec3 a_position, Math::quat a_rotation, Math::vec3 a_scale);

			template<typename T>
			void AddComponent(std::string a_path)
			{
				static_assert(std::is_base_of<Component, T>::value, "Type is not a component");


				if (GetComponent<T>())
				{
					//LOG_ERROR("")
				}


				std::shared_ptr<T> component = std::make_shared<T>();

				component->Create(a_path);

				m_components.emplace_back(component);
			}

			template<typename T>
			std::shared_ptr<T> GetComponent()
			{
				static_assert(std::is_base_of<Component, T>::value, "Type is not a component");

				for (auto it = m_components.begin(); it != m_components.end();)
				{
					std::shared_ptr<T> component = dynamic_cast<std::shared_ptr<T>()>(it);
					std::shared_ptr<T> wantedComponent = std::make_shared<T>();
					if (component == wantedComponent)
					{
						return component;
					}
					else
					{
						++it;
					}
					return nullptr;
				}
			}

			template<typename T>
			void RemoveComponent()
			{
				static_assert(std::is_base_of<Component, T>::value, "Type is not a component");

				for (auto it = m_components.begin(); it != m_components.end();)
				{
					std::shared_ptr<T> component = dynamic_cast<std::shared_ptr<T>()>(it);
					if (component)
					{
						component->Destroy();
						it = m_components.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			std::vector<std::shared_ptr<Component>> GetComponent() { return m_components; }
			Math::Transform GetTransform() { return m_transform; }

		private:
			std::vector<std::shared_ptr<Component>> m_components;
			Math::Transform m_transform;
		};

		
	}
}

#endif 