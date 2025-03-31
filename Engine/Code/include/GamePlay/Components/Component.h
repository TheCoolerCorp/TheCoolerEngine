#ifndef COMPONENT_H
#define COMPONENT_H

#include "EngineExport.h"

#include <iostream>
#include <string>
#include <filesystem>

namespace Engine
{
	namespace GamePlay
	{
		class Component
		{
		public:
			virtual ~Component() = default;
		};
	}
}

#endif 