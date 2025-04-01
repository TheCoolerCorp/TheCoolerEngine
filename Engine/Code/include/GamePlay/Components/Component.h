#ifndef COMPONENT_H
#define COMPONENT_H

#include "EngineExport.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <Refureku/Refureku.h>
#include "Component.rfkh.h"

namespace Engine NAMESPACE()
{
	namespace GamePlay NAMESPACE()
	{
		class CLASS() Component
		{
		public:
			 METHOD() virtual ~Component() = default;

			 Engine_GamePlay_Component_GENERATED
		};
	}
}

File_Component_GENERATED

#endif 