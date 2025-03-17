#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "EngineExport.h"

#include <vector>

#include "Component.h"

namespace Engine
{
	namespace GamePlay
	{
		class GameObject
		{

		private:
			std::vector<Component> m_components;
		};
	}
}

#endif 