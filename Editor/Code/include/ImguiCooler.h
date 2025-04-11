#ifndef IMGUICOOLER_H
#define  IMGUICOOLER_H
#include "Core/Window/IWindow.h"
#include  "Core/Renderer/Renderer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

class UI
{
public:
	void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer);
	void Update(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer);


private:
	VkDescriptorPool m_pool;
	Engine::Core::GraphicsAPI::VulkanRenderPass* m_renderPass = nullptr;
};

#endif