#include  "Core/GraphicsAPI/Vulkan/VulkanRenderObject.h"

#include  "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanPhysicalDevice.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanSurface.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			void VulkanRenderObject::Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ICommandPool* a_commandPool, RHI::IGraphicPipeline* a_graphicPipeline, int a_maxFrame)
			{
				// Create vulkan object from rhi object.
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
				VkDescriptorSetLayout t_descriptorSetLayout = a_graphicPipeline->CastVulkan()->GetObjectDescriptorSetLayout();

				// Create pool
				CreatePool(t_logicalDevice, a_maxFrame);

				// Create descriptorSets.
				CreateDescriptorSets(t_logicalDevice, t_descriptorSetLayout, a_maxFrame);
			}

			void VulkanRenderObject::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{
				// Create vulkan object from rhi object.
				VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();

				vkFreeDescriptorSets(t_logicalDevice, m_pool, static_cast<uint32_t>(m_sets.size()), m_sets.data());
				m_sets.clear();

				vkDestroyDescriptorPool(t_logicalDevice, m_pool, nullptr);
			}

			void VulkanRenderObject::CreatePool(VkDevice a_logicalDevice, int a_maxFrame)
			{
				std::array<VkDescriptorPoolSize, 2> t_poolSizes{};
				t_poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				t_poolSizes[0].descriptorCount = static_cast<uint32_t>(a_maxFrame);
				t_poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				t_poolSizes[1].descriptorCount = static_cast<uint32_t>(a_maxFrame);

				VkDescriptorPoolCreateInfo t_poolInfo{};
				t_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				t_poolInfo.poolSizeCount = static_cast<uint32_t>(t_poolSizes.size());
				t_poolInfo.pPoolSizes = t_poolSizes.data();
				t_poolInfo.maxSets = static_cast<uint32_t>(a_maxFrame);

				VK_CHECK(vkCreateDescriptorPool(a_logicalDevice, &t_poolInfo, nullptr, &m_pool), "failed to create descriptorPool");
			}

			void VulkanRenderObject::CreateDescriptorSets(VkDevice a_logicalDevice, VkDescriptorSetLayout a_descriptorSetLayout, int a_maxFrame)
			{
				std::vector<VkDescriptorSetLayout> layouts(a_maxFrame, a_descriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = m_pool;
				allocInfo.descriptorSetCount = static_cast<uint32_t>(a_maxFrame);
				allocInfo.pSetLayouts = layouts.data();

				m_sets.resize(a_maxFrame);

				VK_CHECK(vkAllocateDescriptorSets(a_logicalDevice, &allocInfo, m_sets.data()), "Can't allocate descriptor sets");
			}
		}
	}
}