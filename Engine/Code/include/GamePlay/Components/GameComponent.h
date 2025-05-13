#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include "EngineExport.h"

#include "GamePlay/Components/Component.h"

namespace Engine::Core::Window
{
	class IInputHandler;
}
namespace Engine::GamePlay
{
	class Scene;

	class GameComponent : public Component
	{
	public:
		ENGINE_API virtual ComponentType Create(int& a_outId) = 0;
		ENGINE_API virtual void Destroy() = 0;

		ENGINE_API virtual void Start() = 0;

		ENGINE_API virtual void Update() = 0;
		ENGINE_API virtual void SceneUpdate() = 0;
		ENGINE_API virtual void ProcessInputs(Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) = 0;

		ENGINE_API void SetScene(Scene* a_scene) { m_scene = a_scene; }
	protected:
		Scene* m_scene = nullptr;

	};
}

#endif // !GAMECOMPONENT_H
