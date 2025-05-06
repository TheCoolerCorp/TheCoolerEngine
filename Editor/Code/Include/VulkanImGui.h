#ifndef VULKANIMGUI_H
#define VULKANIMGUI_H

#include <vector>

#include "RHIImGui.h"
#include "vulkan/vulkan.h"

class ImGuiLayer;

namespace Engine::Core
{
	namespace GraphicsAPI
	{
		class VulkanRenderPass;
	}

	namespace Window
	{
		class IWindow;
	}
	class Renderer;

}

namespace Editor::EditorLayer::Ui
{
	/**
	 * RHI for the GLFW/Vulkan ImGui implementation
	 * Sets up its own renderpass and framebuffer, so that it can capture the scene as an image to be used by the ImGuiLayer
	 */
	class VulkanImGui : public RHIImGui
	{
	public:
		VulkanImGui(Engine::Core::Renderer* a_renderer);
		~VulkanImGui() override;
		void Init(Engine::Core::Window::IWindow* a_window, Engine::Core::Renderer* a_renderer) override;
		void Update() override;

		void SetupRenderPasses();
		void SetupSceneRenderPass();
		void SetupImGuiRenderPass();

		void NewFrame() override;
		void Render() override;
		void DrawSceneAsImage() override;

		void Cleanup() override;

		void CreateDescriptorPool(VkDevice a_device);
		void CreateSceneImageDescriptorSets();
		void RecreateSceneImageDescriptorSets(VkExtent2D a_extent);

		
	private:
		Engine::Core::GraphicsAPI::VulkanRenderPass* m_imGuiRenderPass = nullptr;
		Engine::Core::GraphicsAPI::VulkanRenderPass* m_imGuiViewportRenderPass = nullptr;

		VkExtent2D m_viewportWindowExtent;
		//if resized, recreate necessary elements during update call
		bool m_viewportWindowResized = false;

		std::vector<VkDescriptorSet> m_dset;
		VkDescriptorPool m_pool;

		void SetWindowExtent(VkExtent2D a_extent);
	};
}

#endif