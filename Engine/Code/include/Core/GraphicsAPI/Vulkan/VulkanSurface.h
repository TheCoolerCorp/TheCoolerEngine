#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>

#include "Core/Interfaces/ISurface.h"


namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanSurface : public RHI::ISurface
			{
			public:
				ENGINE_API void Create(Window::IWindow* a_window, RHI::IInstance* a_instance) override;
				ENGINE_API void SetupInfo(RHI::IPhysicalDevice* a_physicalDevice) override;
				ENGINE_API void Destroy(RHI::IInstance* a_instance) override;

				ENGINE_API VulkanSurface* CastVulkan() override { return this; }

				ENGINE_API VkSurfaceKHR& GetVkSurfaceKHR() { return m_surface; }
				ENGINE_API VkSurfaceCapabilitiesKHR GetSurfaceCapabilities() const { return m_capabilities; }
				ENGINE_API std::vector<VkSurfaceFormatKHR> GetSurfaceFormats() const { return m_formats; }
				ENGINE_API std::vector<VkPresentModeKHR> GetSurfacePresentModes() const { return m_presentModes; }

			private:
				VkSurfaceKHR m_surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR m_capabilities{};
				std::vector<VkSurfaceFormatKHR> m_formats;
				std::vector<VkPresentModeKHR> m_presentModes;
			};
		}
	}
}

#endif