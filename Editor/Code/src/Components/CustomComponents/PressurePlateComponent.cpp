#include "../../../Include/Components/CustomComponents/PressurePlateComponent.h"
#include "../../../Include/Components/ComponentRegistry.h"
static inline bool pressurePlateRegistered = Editor::GamePlay::AutoRegisterComponent<Editor::GamePlay::PressurePlateComponent>::registered;

namespace Editor::GamePlay
{
	void PressurePlateComponent::DrawUI()
	{
	}

	void PressurePlateComponent::Start()
	{
		EditorGameComponent::Start();
	}

	void PressurePlateComponent::Update()
	{
		EditorGameComponent::Update();
	}

	void PressurePlateComponent::SceneUpdate()
	{
		EditorGameComponent::SceneUpdate();
	}

	void PressurePlateComponent::ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime)
	{
		EditorGameComponent::ProcessInputs(a_inputHandler, a_deltaTime);
	}
}
