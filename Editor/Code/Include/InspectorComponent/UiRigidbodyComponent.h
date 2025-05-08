#ifndef UIRIGIDBODYCOMPONENT_H
#define UIRIGIDBODYCOMPONENT_H

#include "UiInspectorComponent.h"

namespace Engine::GamePlay
{
	class RigidBodyComponent;
}

namespace Editor::EditorLayer::Ui
{
	/*
	* The Ui Counterpart of the RigidbodyComponent.
	* Allows the user to see but not edit the properties of the MeshComponent.
	*/
	class UiRigidbodyComponent : public InspectorComponent
	{
	public:
		UiRigidbodyComponent(ImGuiLayer* a_layer, Engine::GamePlay::RigidBodyComponent* a_rigidBody)
			: InspectorComponent(a_layer), m_rigidBody(a_rigidBody)
		{
		}

		~UiRigidbodyComponent() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

		UiComponentType GetType() override
		{
			return UiComponentType::RIGIDBODY;
		}
	private:
		Engine::GamePlay::RigidBodyComponent* m_rigidBody = nullptr;

		void UiShowColliderInfo();
		void UiShowBodyType();
		void UiDrawBodyTypeCombo(std::string a_activeComponent);
		void UiShowCollisionLayer();
		void UiDrawCollisionLayerCombo(std::string a_activeComponent);
	};
}

#endif