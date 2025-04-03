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
	static void RenderDrawData(Engine::Core::GraphicsAPI::VkRecordCommandBufferInfo info);
	void Render() override;

	void CreateDescriptorPool(VkDevice device);
	void DrawSceneAsImage() override;

	void ClearDescriptorSets();

	ImDrawData* GetDrawData() override;

	void CreateViewportImageViews();
	void CreateViewportCommandBuffer();
	void CreateViewportFrameBuffers();

	void CreateViewportRenderPass();

	void CreateViewportImage();
	void InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer,
		VkImage image,
		VkAccessFlags srcAccessMask,
		VkAccessFlags dstAccessMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkImageSubresourceRange subresourceRange);

	void SceneRenderPassImGui(Engine::Core::GraphicsAPI::VkRecordCommandBufferInfo info, std::vector<Engine::GamePlay::GameObjectData> objectsData);
	void CreateViewportRenderPass(Engine::Core::GraphicsAPI::VkRecordCommandBufferInfo info, std::vector<Engine::GamePlay::GameObjectData> objectsData);
private:
	Engine::Core::Renderer* m_renderer;
	std::vector<VkDescriptorSet> m_Dset;

	VkDescriptorPool m_pool;
	ImDrawData* m_drawData = nullptr;

	std::vector<VkImage> m_ViewportImages; //done
	std::vector<VkDeviceMemory> m_DstImageMemory; //done
	std::vector<VkImageView> m_ViewportImageViews; //done

	VkRenderPass m_ViewportRenderPass;
	VkPipeline m_ViewportPipeline;
	VkCommandPool m_ViewportCommandPool;
	std::vector<VkFramebuffer> m_ViewportFramebuffers; //done
	std::vector<VkCommandBuffer> m_ViewportCommandBuffers; //done
};