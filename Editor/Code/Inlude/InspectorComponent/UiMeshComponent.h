#ifndef UIMESHCOMPONENT_H
#define UIMESHCOMPONENT_H

#include "UiInspectorComponent.h"

namespace Engine::GamePlay
{
	class MeshComponent;
}

namespace Editor::EditorLayer::Ui
{
	/*
	* The Ui Counterpart of the MeshComponent.
	* Allows the user to see but not edit the properties of the MeshComponent.
	*/
	class UiMeshComponent : public InspectorComponent
	{
	public:
		UiMeshComponent(ImGuiLayer* a_layer, Engine::GamePlay::MeshComponent* a_meshComp)
			: InspectorComponent(a_layer), m_meshComp(a_meshComp)
		{
		}

		~UiMeshComponent() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

	private:
		Engine::GamePlay::MeshComponent* m_meshComp = nullptr;

		VkDescriptorSet m_dSet;
	};
}

#endif