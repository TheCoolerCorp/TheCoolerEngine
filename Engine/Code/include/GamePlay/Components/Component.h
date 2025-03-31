#ifndef COMPONENT_H
#define COMPONENT_H

#include "EngineExport.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <Refureku/Refureku.h>
#include "Refureku/Generated/Component.rfkh.h"

namespace Engine
{
	namespace GamePlay
	{
		class CLASS() Component
		{
		public:
			virtual ~Component() = default;
		};
	}
}

File_Component_GENERATED

#endif 