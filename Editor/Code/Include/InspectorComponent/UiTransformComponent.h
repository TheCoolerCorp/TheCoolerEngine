#ifndef UITRANSFORMCOMPONENT_H
#define UITRANSFORMCOMPONENT_H

#include "UiInspectorComponent.h"

namespace Engine::GamePlay
{
	class TransformComponent;
}

namespace Editor::EditorLayer::Ui
{
	/*
	* The Ui Counterpart of the TransformComponent.
	* Allows the user to see and edit the properties of the TransformComponent.
	*/
	class UiTransformComponent : public InspectorComponent
	{
	public:
		UiTransformComponent(ImGuiLayer* a_layer, Engine::GamePlay::TransformComponent* a_transform)
			: InspectorComponent(a_layer), m_transform(a_transform)
		{
		}

		~UiTransformComponent() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

		UiComponentType GetType() override
		{
			return UiComponentType::TRANSFORM;
		}
	private:
		
		Engine::GamePlay::TransformComponent* m_transform = nullptr;

		Engine::Math::vec3 UtilsToDeg(Engine::Math::vec3 a_euler);
		Engine::Math::vec3 UtilsToRad(Engine::Math::vec3 a_euler);
	};
}


#endif