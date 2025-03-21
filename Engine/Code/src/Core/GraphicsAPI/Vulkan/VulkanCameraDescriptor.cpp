#include "Core/GraphicsAPI/Vulkan/VulkanCameraDescriptor.h"

#include "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanDescriptorPool.h"
#include "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

void Engine::Core::GraphicsAPI::VulkanCameraDescriptor::Create(RHI::ILogicalDevice* a_logicalDevice,
                                                               RHI::IPhysicalDevice* a_physicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool,
                                                               RHI::ICommandPool* a_commandPool, Math::mat4 a_vp, const int a_size)
{
	const VkDevice t_logicalDevice = a_logicalDevice->CastVulkan()->GetVkDevice();
	const VkDescriptorSetLayout t_descriptorSetLayout = a_pipeline->CastVulkan()->GetCameraDescriptorSetLayout();
	const VkDescriptorPool t_descriptorPool = a_descriptorPool->CastVulkan()->GetPool();

	const std::vector<VkDescriptorSetLayout> t_layouts(a_size, t_descriptorSetLayout);
	VkDescriptorSetAllocateInfo t_allocInfo{};
	t_allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	t_allocInfo.descriptorPool = t_descriptorPool;
	t_allocInfo.descriptorSetCount = static_cast<uint32_t>(a_size);
	t_allocInfo.pSetLayouts = t_layouts.data();

	m_descriptorSets.resize(a_size);
	m_uniforms.resize(a_size);

    VK_CHECK(vkAllocateDescriptorSets(t_logicalDevice, &t_allocInfo, m_descriptorSets.data()), "Can't allocate descriptor sets");

    for (size_t i = 0; i < a_size; i++)
    {
        m_uniforms[i] = new VulkanBuffer;

        RHI::BufferData t_uniformData;
        t_uniformData.mUboData = a_vp.mElements.data();
        t_uniformData.mUboSize = 16 * sizeof(float);
        m_uniforms[i]->Create(RHI::BufferType::UBO, t_uniformData, a_physicalDevice, a_logicalDevice, a_commandPool);

        VkDescriptorBufferInfo t_bufferInfo{};
        t_bufferInfo.buffer = m_uniforms[i]->GetBuffer();
        t_bufferInfo.offset = 0;
        t_bufferInfo.range = 16 * sizeof(float);

        VkWriteDescriptorSet t_descriptorWrite;
        t_descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        t_descriptorWrite.dstSet = m_descriptorSets[i];
        t_descriptorWrite.dstBinding = 0;
        t_descriptorWrite.dstArrayElement = 0;
        t_descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        t_descriptorWrite.descriptorCount = 1;
        t_descriptorWrite.pBufferInfo = &t_bufferInfo;

        vkUpdateDescriptorSets(t_logicalDevice, 1, &t_descriptorWrite, 0, nullptr);
    }
}

void Engine::Core::GraphicsAPI::VulkanCameraDescriptor::Destroy(RHI::ILogicalDevice* a_logicalDevice)
{
    for (int i = 0; i < m_uniforms.size(); ++i)
    {
        m_uniforms[i]->Destroy(a_logicalDevice);
    }
    m_descriptorSets.clear();
}

void Engine::Core::GraphicsAPI::VulkanCameraDescriptor::Update(const uint32_t a_frameIndex,
	RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData)
{
    void* t_data;
    vkMapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_frameIndex]->GetMemory(), 0, sizeof(VulkanBuffer), 0, &t_data);
    memcpy(t_data, a_uploadData, sizeof(a_uploadData));
    vkUnmapMemory(a_logicalDevice->CastVulkan()->GetVkDevice(), m_uniforms[a_frameIndex]->GetMemory());
}
