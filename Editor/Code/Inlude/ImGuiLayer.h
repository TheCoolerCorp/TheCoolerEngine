#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Layer.h"
#include "RHIImGui.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer(const std::string& name = "ImGui Layer", Engine::Core::Renderer* renderer) { m_name = name; m_renderer = renderer; }
	~ImGuiLayer() override;
	void OnAttach(Engine::Core::Window::IWindow* window) override;
	void OnDetach() override;
	void OnUpdate(float deltaTime) override;
	void OnUiRender() override;

private:
	RHIImGui* m_imGui = nullptr;
};

#endif