#ifndef GAMECOMPONENTSYSTEM_H
#define GAMECOMPONENTSYSTEM_H

#include <vector>

#include "EngineExport.h"
#include "GamePlay/Systems/ComponentSystem.h"

#include "GamePlay/Components/GameComponent.h"

namespace Engine::GamePlay
{
	class Scene;

	class GameComponentSystem : public Engine::GamePlay::ComponentSystem
	{
	public:
		ENGINE_API GameComponentSystem() = default;
		ENGINE_API ~GameComponentSystem() override = default;

		ENGINE_API void Create(Scene* a_scene, Core::Window::IWindow* a_window);

		ENGINE_API void Update() override;
		ENGINE_API void Start();
		ENGINE_API void SceneUpdate() override;
		ENGINE_API void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;

		ENGINE_API int AddComponent(Engine::GamePlay::GameComponent* a_component);
		ENGINE_API void RemoveComponent(int a_id);
		ENGINE_API Engine::GamePlay::GameComponent* GetComponent(int a_id) const;
	private:
		std::vector<Engine::GamePlay::GameComponent*> m_gameComponents{};
		std::vector<int> m_availableIndexes{};

		Scene* m_scene = nullptr;
		Core::Window::IWindow* m_window = nullptr;
	};
}

#endif // GAMECOMPONENTSYSTEM_H