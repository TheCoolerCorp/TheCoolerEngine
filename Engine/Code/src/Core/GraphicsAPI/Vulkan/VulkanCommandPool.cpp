#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

#include "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/QueueFamilies.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"


namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanCommandPool::Create(RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ILogicalDevice* a_logicalDevice)
			{
				const QueueFamilyIndices t_queueFamilyIndices = QueueFamilyIndices::FindQueueFamilies(a_physicalDevice->CastVulkan()->GetVkPhysicalDevice(), a_surface->CastVulkan()->GetVkSurfaceKHR());

				VkCommandPoolCreateInfo t_poolInfo{};
				t_poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				t_poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
				t_poolInfo.queueFamilyIndex = t_queueFamilyIndices.GetGraphicsFamily().value();

				VK_CHECK(vkCreateCommandPool(a_logicalDevice->CastVulkan()->GetVkDevice(), &t_poolInfo, nullptr, &m_commandPool), "failed to create command pool!");
			}
		}
	}
}
