#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

#include "EngineExport.h"
#include "System.h"
#include <unordered_map>
#include "GamePlay/Components/Component.h"


namespace Engine::Core::Window
{
	class IInputHandler;
}

namespace Engine::GamePlay
{
	class ComponentSystem : public Engine::GamePlay::System
	{
	public:
		ComponentSystem() = default;
		~ComponentSystem() override = default;

		void Update(float a_deltatime) override {}
		void SceneUpdate() override {}
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override {}

	private:
		std::unordered_map<const char*, std::vector<Engine::GamePlay::Component*>> m_components;
	};
}
#endif