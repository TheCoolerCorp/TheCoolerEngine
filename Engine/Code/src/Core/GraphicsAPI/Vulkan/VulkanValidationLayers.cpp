#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"

#include "Core/Assertion/Assertion.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanValidationLayers::Create(RHI::IInstance* a_instance)
			{
				Setup(a_instance->CastVulkan()->GetVkInstance());
			}

			void VulkanValidationLayers::Setup(VkInstance a_instance)
			{
				if constexpr (!RHI::ENABLEVALIDATIONLAYERS)
				{
					return;
				}

				VkDebugUtilsMessengerCreateInfoEXT createInfo;
				PopulateDebugMessengerCreateInfo(createInfo);

				VK_CHECK(CreateDebugUtilsMessengerEXT(a_instance, &createInfo, nullptr, &m_debugMessenger), "failed to set up debug messenger!")

				Check();
			}

			void VulkanValidationLayers::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& a_createInfo)
			{
				a_createInfo = {};
				a_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				a_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
				a_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
				a_createInfo.pfnUserCallback = DebugCallback;
			}

			void VulkanValidationLayers::Destroy(RHI::IInstance* a_instance)
			{
				if (RHI::ENABLEVALIDATIONLAYERS) 
				{
					DestroyDebugUtilsMessengerEXT(a_instance->CastVulkan()->GetVkInstance(), m_debugMessenger, nullptr);
				}

			}

			void VulkanValidationLayers::Check()
			{
				if (!CheckValidationLayerSupport())
				{
					LOG_ERROR("validation layers requested, but not available!");
				}
			}

			VKAPI_ATTR VkBool32 VKAPI_CALL VulkanValidationLayers::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity, VkDebugUtilsMessageTypeFlagsEXT a_messageType, const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData, void* a_pUserData)
			{
				std::string messageType;

				if (a_messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
					messageType = "GENERAL";
				}
				else if (a_messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
					messageType = "VALIDATION";
				}
				else if (a_messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
					messageType = "PERFORMANCE";
				}

				if (a_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
					LOG_VERBOSE("[Vulkan] : Validation layers: " + messageType + " " + Debugging::ToString(a_pCallbackData->pMessage) + '\n');
				}
				else if (a_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
					LOG_INFO("[Vulkan] : Validation layers: " + messageType + " " + Debugging::ToString(a_pCallbackData->pMessage) + '\n');
				}
				else if (a_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
					LOG_WARNING("[Vulkan] : Validation layers: " + messageType + " " + Debugging::ToString(a_pCallbackData->pMessage) + '\n');
				}
				else if (a_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
					LOG_ERROR("[Vulkan] : Validation layers: " + messageType + " " + Debugging::ToString(a_pCallbackData->pMessage) + '\n');
				}

				return VK_FALSE;
			}

			VkResult VulkanValidationLayers::CreateDebugUtilsMessengerEXT(VkInstance a_instance, const VkDebugUtilsMessengerCreateInfoEXT* a_pCreateInfo, const VkAllocationCallbacks* a_pAllocator, VkDebugUtilsMessengerEXT* a_pDebugMessenger)
			{
				const auto t_func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
					vkGetInstanceProcAddr(a_instance, "vkCreateDebugUtilsMessengerEXT"));
				if (t_func != nullptr)
				{
					return t_func(a_instance, a_pCreateInfo, a_pAllocator, a_pDebugMessenger);
				}
				
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}

			void VulkanValidationLayers::DestroyDebugUtilsMessengerEXT(VkInstance a_instance, VkDebugUtilsMessengerEXT a_debugMessenger, const VkAllocationCallbacks* a_pAllocator)
			{
				const auto t_func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
					vkGetInstanceProcAddr(a_instance, "vkDestroyDebugUtilsMessengerEXT"));
				if (t_func != nullptr)
				{
					t_func(a_instance, a_debugMessenger, a_pAllocator);
				}

			}

			bool VulkanValidationLayers::CheckValidationLayerSupport()
			{
				uint32_t t_layerCount;
				vkEnumerateInstanceLayerProperties(&t_layerCount, nullptr);

				std::vector<VkLayerProperties> t_availableLayers(t_layerCount);
				vkEnumerateInstanceLayerProperties(&t_layerCount, t_availableLayers.data());

				for (const char* t_layerName : VALIDATION_LAYERS) {
					bool t_layerFound = false;

					for (const auto& t_layerProperties : t_availableLayers) {
						if (strcmp(t_layerName, t_layerProperties.layerName) == 0) {
							t_layerFound = true;
							break;
						}
					}

					if (!t_layerFound) {
						return false;
					}
				}

				return true;
			}
		}
	}
}
