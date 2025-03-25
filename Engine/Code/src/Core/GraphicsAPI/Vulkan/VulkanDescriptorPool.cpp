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
			void VulkanDescriptorPool::Create(RHI::ILogicalDevice* a_logicalDevice, const int a_maxFrameInFlight)
			{
                m_maxFrame = a_maxFrameInFlight;
                std::array<VkDescriptorPoolSize, 2> t_poolSizes{};
                t_poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                t_poolSizes[0].descriptorCount = static_cast<uint32_t>(m_maxFrame);
                t_poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                t_poolSizes[1].descriptorCount = static_cast<uint32_t>(m_maxFrame);

                VkDescriptorPoolCreateInfo t_poolInfo{};
                t_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                t_poolInfo.poolSizeCount = static_cast<uint32_t>(t_poolSizes.size());
                t_poolInfo.pPoolSizes = t_poolSizes.data();
                t_poolInfo.maxSets = static_cast<uint32_t>(m_maxFrame);

				VK_CHECK(vkCreateDescriptorPool(a_logicalDevice->CastVulkan()->GetVkDevice(), &t_poolInfo, nullptr, &m_pool), "failed to create descriptorPool");
			}
			void VulkanDescriptorPool::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				vkDestroyDescriptorPool(a_logicalDevice->CastVulkan()->GetVkDevice(), m_pool, nullptr);
			}
		}
	}
}
