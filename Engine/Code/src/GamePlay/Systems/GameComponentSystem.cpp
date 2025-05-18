#include "GamePlay/Systems/GameComponentSystem.h"
#include "GamePlay/Others/Scene.h"
#include "GamePlay/Systems/EventSystem.h"
namespace Engine::GamePlay
{
	void GameComponentSystem::Create(Scene* a_scene, Core::Window::IWindow* a_window)
	{
		m_gameComponents.reserve(50);
		m_scene = a_scene;
		m_window = a_window;
		a_scene->GetBeginPlayEvent().AddListener([this]() {this->Start();});
	}

	void GameComponentSystem::Update(float a_deltatime)
	{
		for (auto& component : m_gameComponents)
		{
			component->Update(a_deltatime);
		}
	}
	void GameComponentSystem::Start()
	{
		for (auto& component : m_gameComponents)
		{
			if (!component)
				continue;
			component->Start();
		}
	}
	void GameComponentSystem::SceneUpdate()
	{
		for (auto& component : m_gameComponents)
		{
			if (!component)
				continue;
			component->SceneUpdate();
		}
	}
	void GameComponentSystem::ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
	{
		for (auto& component : m_gameComponents)
		{
			if (!component)
				continue;
			component->ProcessInputs(a_inputHandler, a_deltaTime);
		}
	}
	int GameComponentSystem::AddComponent(Engine::GamePlay::GameComponent* a_component)
	{
		a_component->SetScene(m_scene);
		a_component->SetWindow(m_window);
		if (m_availableIndexes.empty())
		{
			m_gameComponents.push_back(a_component);
			return static_cast<int>(m_gameComponents.size()) - 1;
		}
		int t_index = m_availableIndexes.back();
		m_availableIndexes.pop_back();
		m_gameComponents[t_index] = a_component;
		return t_index;
	}
	void GameComponentSystem::RemoveComponent(int a_id)
	{
		if (a_id >= 0 && a_id < m_gameComponents.size())
		{
			delete m_gameComponents[a_id];
			m_gameComponents[a_id] = nullptr;
			m_availableIndexes.push_back(a_id);
		}
	}

	Engine::GamePlay::GameComponent* GameComponentSystem::GetComponent(int a_id) const
	{
		if (a_id >= 0 && a_id < m_gameComponents.size())
		{
			return m_gameComponents[a_id];
		}
		return nullptr;
	}
}
