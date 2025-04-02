#ifndef COMPONENT_H
#define COMPONENT_H

#include "EngineExport.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <Refureku/Refureku.h>
//#include "Component.rfkh.h"

namespace Engine
{
	namespace GamePlay
	{
		class Component
		{
		public:
			virtual ~Component() = default;

			//ENGINE_API virtual void Create(int* a_id) = 0;
		};
	}
}

//File_Component_GENERATED

#endif 