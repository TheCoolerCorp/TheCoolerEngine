#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

#include "EngineExport.h"
#include "System.h"
#include <unordered_map>
#include "GamePlay/Components/Component.h"


namespace Engine::GamePlay
{
	class Component;

	class ComponentSystem : public Engine::GamePlay::System
	{
	public:
		ComponentSystem() = default;
		~ComponentSystem() override = default;

		void Update() override;
		void SceneUpdate() override;
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;

	private:
		std::unordered_map<const char*, std::vector<Engine::GamePlay::Component*>> m_components;
	};
}



#endif