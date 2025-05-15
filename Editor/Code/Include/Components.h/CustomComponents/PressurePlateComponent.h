#ifndef PRESSUREPLATECOMPONENT_H
#define PRESSUREPLATECOMPONENT_H

#include "../EditorGameComponent.h"
#include "../ComponentRegistry.h"

namespace Editor::GamePlay
{
	class PressurePlateComponent : public EditorGameComponent, public AutoRegisterComponent<PressurePlateComponent>
	{

	};
}

#endif // !PRESSUREPLATECOMPONENT_H