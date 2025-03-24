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

	void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer) override;

	void NewFrame() override;
	static void RenderDrawData(VkCommandBuffer commandBuffer);
	void Render() override;

	void CreateDescriptorPool(VkDevice device);

	ImDrawData* GetDrawData() override;
private:
	VkDescriptorPool m_pool;
	ImDrawData* m_drawData = nullptr;
};