#pragma once
#include "RHIImGuiRenderer.h"

class VulkanImGuiRenderer : RHIImGuiRenderer
{
public:
	void Init(IWindow* window, Renderer* renderer) override;

	void NewFrame() override;
	void Render() override;
};