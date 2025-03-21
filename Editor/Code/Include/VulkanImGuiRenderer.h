#pragma once
#include "RHIImGuiRenderer.h"

//ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

class VulkanImGuiRenderer : public RHIImGuiRenderer
{
public:
	VulkanImGuiRenderer() = default;

	void Init(IWindow* window, Renderer* renderer) override;

	void NewFrame() override;
	void Render() override;
	ImDrawData* GetDrawData() override;
private:
	ImDrawData* m_drawData = nullptr;
};