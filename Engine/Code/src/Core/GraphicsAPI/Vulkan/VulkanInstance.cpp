#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"

#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"
#include "GLFW/glfw3.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
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

				std::vector<const char*> t_extensions = GetRequiredExtensions();
				createInfo.enabledExtensionCount = static_cast<uint32_t>(t_extensions.size());
				createInfo.ppEnabledExtensionNames = t_extensions.data();
				t_extensions.clear();

				if (RHI::ENABLEVALIDATIONLAYERS) 
				{
					createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
					createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
				}
				else 
				{
					createInfo.enabledLayerCount = 0;
				}

				VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_Instance), "failed to create instance");
			}

			void VulkanInstance::Destroy()
			{
				vkDestroyInstance(m_Instance, nullptr);

			}

			std::vector<const char*> VulkanInstance::GetRequiredExtensions()
			{
				uint32_t t_glfwExtensionCount = 0;
				const char** t_glfwExtensions = glfwGetRequiredInstanceExtensions(&t_glfwExtensionCount); // TODO : replace with IWindow function

				std::vector<const char*> extensions(t_glfwExtensions, t_glfwExtensions + t_glfwExtensionCount);

				if (RHI::ENABLEVALIDATIONLAYERS)
				{
					extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				}

				return extensions;
			}
		}
	}
}
