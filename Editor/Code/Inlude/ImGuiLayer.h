#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer(const std::string& name = "UnnamedLayer", Engine::Core::Renderer* renderer) { m_name = name; m_renderer = renderer; }
	~ImGuiLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float deltaTime) override;
	void OnUiRender() override;
};

#endif