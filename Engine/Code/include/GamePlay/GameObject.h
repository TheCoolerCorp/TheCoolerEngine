#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "EngineExport.h"

#include <vector>
#include <string>
#include "Component.h"

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