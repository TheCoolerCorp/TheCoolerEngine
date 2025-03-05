#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanValidationLayers::Create(RHI::IInstance* a_instance)
			{
				
			}

			void VulkanValidationLayers::Setup(VkInstance a_instance)
			{
				
			}

			void VulkanValidationLayers::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& a_createInfo)
			{
				
			}

			void VulkanValidationLayers::Destroy(RHI::IInstance* a_instance)
			{
				
			}

			void VulkanValidationLayers::Check()
			{
				
			}


			VKAPI_ATTR VkBool32 VKAPI_CALL VulkanValidationLayers::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity, VkDebugUtilsMessageTypeFlagsEXT a_messageType, const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData, void* a_pUserData)
			{
				
			}

			VkResult VulkanValidationLayers::CreateDebugUtilsMessengerEXT(VkInstance a_instance, const VkDebugUtilsMessengerCreateInfoEXT* a_pCreateInfo, const VkAllocationCallbacks* a_pAllocator, VkDebugUtilsMessengerEXT* a_pDebugMessenger)
			{
				
			}

			void VulkanValidationLayers::DestroyDebugUtilsMessengerEXT(VkInstance a_instance, VkDebugUtilsMessengerEXT a_debugMessenger, const VkAllocationCallbacks* a_pAllocator)
			{
				
			}

			bool VulkanValidationLayers::CheckValidationLayerSupport()
			{
				
			}


		}
	}
}