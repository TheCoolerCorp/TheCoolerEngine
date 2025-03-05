#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"

namespace Engine::Core::GraphicsAPI
{
	void VulkanInstance::Create()
	{
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "The Cooler Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pEngineName = "The Cooler Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance), "failed to create instance");
	}

	void VulkanInstance::Destroy()
	{
        vkDestroyInstance(m_instance, nullptr);

	}
}
