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
				VkApplicationInfo t_appInfo{};
				t_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				t_appInfo.pApplicationName = "The Cooler Engine";
				t_appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
				t_appInfo.pEngineName = "The Cooler Engine";
				t_appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
				t_appInfo.apiVersion = VK_API_VERSION_1_0;

				VkInstanceCreateInfo t_createInfo{};
				t_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				t_createInfo.pApplicationInfo = &t_appInfo;

				std::vector<const char*> t_extensions = GetRequiredExtensions();
				t_createInfo.enabledExtensionCount = static_cast<uint32_t>(t_extensions.size());
				t_createInfo.ppEnabledExtensionNames = t_extensions.data();
				t_extensions.clear();

				if (RHI::ENABLEVALIDATIONLAYERS) 
				{
					t_createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
					t_createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
				}
				else 
				{
					t_createInfo.enabledLayerCount = 0;
				}

				VK_CHECK(vkCreateInstance(&t_createInfo, nullptr, &m_instance), "failed to create instance");
			}

			void VulkanInstance::Destroy()
			{
				vkDestroyInstance(m_instance, nullptr);
			}

			std::vector<const char*> VulkanInstance::GetRequiredExtensions()
			{
				uint32_t t_glfwExtensionCount = 0;
				const char** t_glfwExtensions = glfwGetRequiredInstanceExtensions(&t_glfwExtensionCount); // TODO : replace with IWindow function

				std::vector<const char*> t_extensions(t_glfwExtensions, t_glfwExtensions + t_glfwExtensionCount);

				if (RHI::ENABLEVALIDATIONLAYERS)
				{
					t_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				}

				return t_extensions;
			}
		}
	}
}
