#ifndef VULKANVALIDATIONLAYERS_H
#define VULKANVALIDATIONLAYERS_H

#include "VulkanUtils.h"
#include <vector>

#include  "EngineExport.h"
#include "Core/Interfaces/IValidationLayers.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			const std::vector<const char*> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

			class ENGINE_API VulkanValidationLayers : public RHI::IValidationLayers
			{
			public:
				~VulkanValidationLayers() override = default;

				void Create(RHI::IInstance* a_instance) override;
				void Setup(VkInstance a_instance);
				static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& a_createInfo);
				void Destroy(RHI::IInstance* a_instance) override;
				static void Check();
				VulkanValidationLayers* CastVulkan() override { return this; }

			private:
				VkDebugUtilsMessengerEXT m_debugMessenger{};

				static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity, VkDebugUtilsMessageTypeFlagsEXT a_messageType, const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData, void* a_pUserData);
				static VkResult CreateDebugUtilsMessengerEXT(VkInstance a_instance, const VkDebugUtilsMessengerCreateInfoEXT* a_pCreateInfo, const VkAllocationCallbacks* a_pAllocator, VkDebugUtilsMessengerEXT* a_pDebugMessenger);
				static void DestroyDebugUtilsMessengerEXT(VkInstance a_instance, VkDebugUtilsMessengerEXT a_debugMessenger, const VkAllocationCallbacks* a_pAllocator);
				static bool CheckValidationLayerSupport();
			};
		}
	}
}

#endif

//
//#include <vector>
//#include <vulkan/vulkan_core.h>
//
//#include "EngineExport.h"
//#include "Core/Interfaces/IValidationLayers.h"
//
//namespace Engine
//{
//	namespace Core
//	{
//		namespace GraphicsAPI
//		{
//            const std::vector<const char*> validationLayers = {
//				"VK_LAYER_KHRONOS_validation"
//            };
//
//            class ENGINE_API VulkanValidationLayers : public RHI::IValidationLayers
//            {
//            public:
//                ~VulkanValidationLayers() override = default;
//                void Create(RHI::IInstance* a_instance) override;
//                void Setup(VkInstance a_instance);
//                void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& a_createInfo);
//                void Destroy(RHI::IInstance* a_instance) override;
//                void Check();
//                VulkanValidationLayers* CastVulkan() override;
//
//            private:
//                VkDebugUtilsMessengerEXT m_debugMessenger{};
//
//                static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity, VkDebugUtilsMessageTypeFlagsEXT a_messageType, const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData, void* a_pUserData);
//                static VkResult CreateDebugUtilsMessengerEXT(VkInstance a_instance, const VkDebugUtilsMessengerCreateInfoEXT* a_pCreateInfo, const VkAllocationCallbacks* a_pAllocator, VkDebugUtilsMessengerEXT* a_pDebugMessenger);
//                static void DestroyDebugUtilsMessengerEXT(VkInstance a_instance, VkDebugUtilsMessengerEXT a_debugMessenger, const VkAllocationCallbacks* a_pAllocator);
//                bool CheckValidationLayerSupport();
//            };
//		}
//	}
//}
//
//#endif