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
			const std::vector<const char*> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor"};

			class VulkanValidationLayers : public RHI::IValidationLayers
			{
			public:
				ENGINE_API ~VulkanValidationLayers() override = default;

				ENGINE_API void Create(RHI::IInstance* a_instance) override;
				ENGINE_API void Setup(VkInstance a_instance);
				ENGINE_API static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& a_createInfo);
				ENGINE_API void Destroy(RHI::IInstance* a_instance) override;
				ENGINE_API static void Check();
				ENGINE_API VulkanValidationLayers* CastVulkan() override { return this; }

			private:
				VkDebugUtilsMessengerEXT m_debugMessenger{};

				ENGINE_API static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity, VkDebugUtilsMessageTypeFlagsEXT a_messageType, const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData, void* a_pUserData);
				ENGINE_API static VkResult CreateDebugUtilsMessengerEXT(VkInstance a_instance, const VkDebugUtilsMessengerCreateInfoEXT* a_pCreateInfo, const VkAllocationCallbacks* a_pAllocator, VkDebugUtilsMessengerEXT* a_pDebugMessenger);
				ENGINE_API static void DestroyDebugUtilsMessengerEXT(VkInstance a_instance, VkDebugUtilsMessengerEXT a_debugMessenger, const VkAllocationCallbacks* a_pAllocator);
				ENGINE_API static bool CheckValidationLayerSupport();
			};
		}
	}
}

#endif