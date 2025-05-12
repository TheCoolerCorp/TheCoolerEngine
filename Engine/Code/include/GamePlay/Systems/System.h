#ifndef SYSTEM_H
#define SYSTEM_H

#include "EngineExport.h"


namespace Engine::Core::Window
{
	class IInputHandler;
}

namespace Engine
{
	namespace GamePlay
	{
		class System
		{
		public:
			ENGINE_API System() = default;
			ENGINE_API virtual ~System() = default;

			ENGINE_API virtual void Update() {}
			ENGINE_API virtual void SceneUpdate() {}
			ENGINE_API virtual void ProcessInputs(Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) {}
		};
	}
}
#endif