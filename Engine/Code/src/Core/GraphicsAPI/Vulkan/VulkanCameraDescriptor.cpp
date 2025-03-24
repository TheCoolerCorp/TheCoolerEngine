#include "Core/GraphicsAPI/Vulkan/VulkanCameraDescriptor.h"

#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

void Engine::Core::GraphicsAPI::VulkanCameraDescriptor::Create(RHI::ILogicalDevice* a_logicalDevice,
                                                               RHI::IPhysicalDevice* a_physicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool,
                                                               RHI::ICommandPool* a_commandPool, Math::mat4 a_vp)
{
	const VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
	const VkDescriptorSetLayout t_descriptorSetLayout = a_pipeline->CastVulkan()->GetCameraDescriptorSetLayout();
	const VkDescriptorPool t_descriptorPool = a_descriptorPool->CastVulkan()->GetPool();

    const std::array<VkDescriptorSetLayout, 1> t_layouts = { t_descriptorSetLayout };
	VkDescriptorSetAllocateInfo t_allocInfo{};
	t_allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	t_allocInfo.descriptorPool = t_descriptorPool;
	t_allocInfo.descriptorSetCount = 1u;
	t_allocInfo.pSetLayouts = t_layouts.data();


    VK_CHECK(vkAllocateDescriptorSets(t_logicalDevice, &t_allocInfo, &m_descriptorSet), "Can't allocate descriptor sets");

    m_uniform = new VulkanBuffer;

    RHI::BufferData t_uniformData;
    t_uniformData.mUboData = a_vp.mElements.data();
    t_uniformData.mUboSize = 16 * sizeof(float);
    m_uniform->Create(RHI::BufferType::UBO, t_uniformData, a_physicalDevice, a_logicalDevice, a_commandPool);

    VkDescriptorBufferInfo t_bufferInfo{};
    t_bufferInfo.buffer = m_uniform->GetBuffer();
    t_bufferInfo.offset = 0;
    t_bufferInfo.range = 16 * sizeof(float);

    std::array<VkWriteDescriptorSet, 1> t_descriptorWrites{};

    t_descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    t_descriptorWrites[0].dstSet = m_descriptorSet;
    t_descriptorWrites[0].dstBinding = 0;
    t_descriptorWrites[0].dstArrayElement = 0;
    t_descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    t_descriptorWrites[0].descriptorCount = 1;
    t_descriptorWrites[0].pBufferInfo = &t_bufferInfo;

    vkUpdateDescriptorSets(t_logicalDevice, 1, t_descriptorWrites.data(), 0, nullptr);
}

void Engine::Core::GraphicsAPI::VulkanCameraDescriptor::Destroy(RHI::ILogicalDevice* a_logicalDevice)
{
	m_uniform->Destroy(a_logicalDevice);
}

void Engine::Core::GraphicsAPI::VulkanCameraDescriptor::Update(RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData) 
{
    void* t_data;
    vkMapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniform->GetMemory(), 0, sizeof(VulkanBuffer), 0, &t_data);
    memcpy(t_data, a_uploadData, 16 * sizeof(float));
    vkUnmapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniform->GetMemory());
}
