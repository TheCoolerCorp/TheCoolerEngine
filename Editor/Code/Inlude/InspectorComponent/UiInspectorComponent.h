#ifndef UIINSPECTORCOMPONENT_H
#define UIINSPECTORCOMPONENT_H
#include "../ImGuiLayer.h"

namespace Editor::EditorLayer::Ui
{
	/*
	* InspectorComponent is an abstract parent class for all inspector components.
	* It handles its own draw logic but is meant to be drawn in a preexisting window as a sort of child element.
	*/
	class InspectorComponent
	{
	public:
		InspectorComponent(ImGuiLayer* a_layer) :m_layer(a_layer) {}
		virtual ~InspectorComponent() = default;
		virtual void Create() = 0;
		virtual void UiDraw() = 0;
		virtual void Destroy() = 0;

		/**
		 * Set the unique id of the Inspector Component.
		 * This id is very important for the imgui context and not setting it will cause problems
		 * @param a_uid the unique id
		 */
		void SetUid(int a_uid) { m_uid = a_uid; }
	protected:
		int m_uid;

		ImGuiLayer* m_layer = nullptr;
	};
}

#endif