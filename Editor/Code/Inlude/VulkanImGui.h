#ifndef VULKANIMGUI_H
#define VULKANIMGUI_H

#include <vector>

#include "RHIImGui.h"
#include "vulkan/vulkan.h"

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

class VulkanImGui : public RHIImGui
{
public:
	VulkanImGui(Engine::Core::Renderer* renderer);
	~VulkanImGui() override = default;
	void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer) override;
	void SetupRenderPasses();
	void SetupSceneRenderPass();
	void SetupImGuiRenderPass();

	void NewFrame() override;
	void Render() override;
	void DrawSceneAsImage() override;


	void CreateDescriptorPool(VkDevice device);
private:
	Engine::Core::Renderer* m_renderer = nullptr;

	Engine::Core::GraphicsAPI::VulkanRenderPass* m_imGuiRenderPass = nullptr;

	std::vector<VkDescriptorSet> m_Dset;
	VkDescriptorPool m_pool;
};

#endif