#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include "EngineExport.h"

#include "GamePlay/Components/Component.h"

namespace Engine::GamePlay
{
	class GameComponent : public Engine::GamePlay::Component
	{
	public:
		ENGINE_API virtual ComponentType Create(int& a_outId) override;
		ENGINE_API virtual void Destroy() override;

		ENGINE_API virtual void Update();
		ENGINE_API virtual void SceneUpdate();
		ENGINE_API virtual void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;


	}
}

#endif // !GAMECOMPONENT_H
