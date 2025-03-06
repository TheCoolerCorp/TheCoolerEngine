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
			class ENGINE_API VulkanSurface : public RHI::ISurface
			{
			public:
				struct SurfaceInfo
				{
					VkSurfaceCapabilitiesKHR capabilities;
					std::vector<VkSurfaceFormatKHR> formats;
					std::vector<VkPresentModeKHR> presentModes;
					void QueryInfo();
				};
				void Create(Window::IWindow* a_window, RHI::IInstance* a_instance) override;
				void SetupInfo(RHI::IPhysicalDevice* a_physicalDevice) override;
				void Destroy(RHI::IInstance* a_instance) override;

				VulkanSurface* CastVulkan() override { return this; }

				VkSurfaceKHR& GetVkSurfaceKHR() { return m_surface; }
				SurfaceInfo GetSurfaceInfo() { return m_info; }

			private:
				VkSurfaceKHR m_surface = VK_NULL_HANDLE;
				SurfaceInfo m_info;
			};
		}
	}
}

#endif