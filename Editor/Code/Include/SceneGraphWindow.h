#ifndef SCENEGRAPHUIWINDOW_H
#define SCENEGRAPHUIWINDOW_H

#include "UiWindow.h"

#include "Core/Renderer/Renderer.h"
#include "GamePlay/Others/Scene.h"

namespace Editor::EditorLayer::Ui
{
	class SceneGraphUiWindow : public UiWindow
	{
	public:
		SceneGraphUiWindow(Engine::Core::Renderer* a_renderer, ImGuiLayer* a_layer, Engine::GamePlay::Scene* a_scene)
			: UiWindow(a_renderer, a_layer)
			, m_scene(a_scene)
		{
			m_name = "Scene Graph";
			//so that we dont need to do a ton of redundant calls to the service locator
			m_transformSystem = Engine::GamePlay::ServiceLocator::GetTransformSystem();
		}
		~SceneGraphUiWindow() override;
		void Create() override;
		void UiDraw() override;
		void ProcessInputs(Engine::Core::Window::IInputHandler* a_inputHandler, float a_deltaTime) override;
		void Destroy() override;

		void NotifyObjectRemoved(Engine::GamePlay::GameObject* a_object) override;
	private:
		Engine::GamePlay::Scene* m_scene;
		Engine::GamePlay::TransformSystem* m_transformSystem = nullptr;

		void UiDrawObject(int a_transformId);
		void UiCreateNameTextField(Engine::GamePlay::GameObject* a_object);
		void UiAddPopupContext(Engine::GamePlay::GameObject* a_object);

		//drag/drop functionality for rearanging objects in the hierarchy
		void UiAddDragDropSource(Engine::GamePlay::GameObject* a_object);
		void UiAddObjectDragDropTarget(Engine::GamePlay::GameObject* a_object);
		void UiAddRootDragDropTarget();
	};
}
#endif