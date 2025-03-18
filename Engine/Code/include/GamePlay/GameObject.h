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




			template<typename T>
			void AddComponent(std::string a_path, std::string a_name)
			{
				// Add security for non Component type
				if (!std::is_base_of<Component, T>::value)
				{
					LOG_ERROR("This is not a resource");
					return;
				}
				T* component = new T();

				component->Create(a_path);

				m_components.push_back(component);
			}

		private:
			std::vector<Component*> m_components;
		};

		
	}
}

#endif 