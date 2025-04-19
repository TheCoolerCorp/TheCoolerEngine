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
	class VulkanImGui : public RHIImGui
	{
	public:
		VulkanImGui(Engine::Core::Renderer* a_renderer);
		~VulkanImGui() override;
		void Init(Engine::Core::Window::IWindow* a_window, Engine::Core::Renderer* a_renderer) override;

		void SetupRenderPasses();
		void SetupSceneRenderPass();
		void SetupImGuiRenderPass();

		void NewFrame() override;
		void Render() override;
		void DrawSceneAsImage() override;

		void Cleanup() override;

		void CreateDescriptorPool(VkDevice a_device);
		void CreateSceneImageDescriptorSets();
	private:
		Engine::Core::GraphicsAPI::VulkanRenderPass* m_imGuiRenderPass = nullptr;
		Engine::Core::GraphicsAPI::VulkanRenderPass* m_imGuiViewportRenderPass = nullptr;

		std::vector<VkDescriptorSet> m_dset;
		VkDescriptorPool m_pool;
	};
}

#endif