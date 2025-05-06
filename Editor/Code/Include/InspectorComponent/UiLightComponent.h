#ifndef UILIGHTCOMPONENT_H
#define UILIGHTCOMPONENT_H

#include "UiInspectorComponent.h"

namespace Engine::GamePlay
{
	class LightComponent;
}
namespace Editor::EditorLayer::Ui
{
	class UiLightComponent : public InspectorComponent
	{
	public:
		UiLightComponent(ImGuiLayer* a_layer, Engine::GamePlay::LightComponent* a_lightComp)
			: InspectorComponent(a_layer), m_lightComp(a_lightComp)
		{
		}

		~UiLightComponent() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

		UiComponentType GetType() override
		{
			return UiComponentType::LIGHT;
		}
	private:
		Engine::GamePlay::LightComponent* m_lightComp = nullptr;
	};
}

#endif