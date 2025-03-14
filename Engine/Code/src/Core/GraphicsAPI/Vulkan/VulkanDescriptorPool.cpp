#include  "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include <array>

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/Interfaces/ILogicalDevice.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanDescriptorPool::Create(RHI::ILogicalDevice* a_logicalDevice, int a_maxFrameInFlight)
			{
                m_maxFrame = a_maxFrameInFlight;
                std::array<VkDescriptorPoolSize, 2> poolSizes{};
                poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                poolSizes[0].descriptorCount = static_cast<uint32_t>(m_maxFrame);
                poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                poolSizes[1].descriptorCount = static_cast<uint32_t>(m_maxFrame);

                VkDescriptorPoolCreateInfo poolInfo{};
                poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
                poolInfo.pPoolSizes = poolSizes.data();
                poolInfo.maxSets = static_cast<uint32_t>(m_maxFrame);

				VK_CHECK(vkCreateDescriptorPool(a_logicalDevice->CastVulkan()->GetVkDevice(), &poolInfo, nullptr, &m_pool), "failed to create descriptorPool");
			}
			void VulkanDescriptorPool::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				vkDestroyDescriptorPool(a_logicalDevice->CastVulkan()->GetVkDevice(), m_pool, nullptr);
			}
		}
	}
}
