#ifndef VULKANRENDEROBJECT_H
#define VULKANRENDEROBJECT_H

#include "EngineExport.h"

#include "Core/Interfaces/IObjectDescriptor.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanImage.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanGraphicPipeline.h"

#include <vector>
#include <array>

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanObjectDescriptor : public RHI::IObjectDescriptor
			{
			public:
				ENGINE_API ~VulkanObjectDescriptor() override = default;

				ENGINE_API VulkanObjectDescriptor* CastVulkan() override { return this; }

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IGraphicPipeline* a_graphicPipeline, RHI::DescriptorSetTarget a_type, uint32_t a_maxDescriptorPerSet, uint32_t a_setCount, std::vector<uint32_t> a_subSetCount, uint32_t a_uniformCount, std::vector<uint32_t> a_subUniformCount, std::vector<RHI::DescriptorSetDataType> a_types) override;

				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API void SetTexture(RHI::ILogicalDevice* a_logicalDevice, uint32_t a_setIndex, RHI::IImage* a_image, uint32_t a_dstBinding, uint32_t a_count) override;
				ENGINE_API void SetUniform(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, uint32_t a_setindex, uint32_t a_bufferIndex, void* a_data, uint32_t a_dataSize, uint32_t a_dstBinding, uint32_t a_descriptorCount) override;

				ENGINE_API void UpdateUniforms(RHI::ILogicalDevice* a_logicalDevice, uint32_t a_bufferIndex, void* a_data, uint32_t a_dataSize, int a_imageIndex) override;


				ENGINE_API std::vector<VkDescriptorSet> GetDescriptorSets(uint32_t a_setIndex) const { return m_sets[a_setIndex]; }
				ENGINE_API int GetSetsCount() const { return static_cast<int>(m_sets.size()); }
				ENGINE_API RHI::DescriptorSetTarget GetType() { return m_type; }
				ENGINE_API RHI::DescriptorSetPipelineTarget GetPipelineTargetType() override { return m_pipelineType; }

				ENGINE_API bool IsUpdated() override { return m_Updated; }
			private:
				VkDescriptorPool m_pool = VK_NULL_HANDLE;

				std::vector<std::vector<VkDescriptorSet>> m_sets;
				std::vector<std::vector<VulkanBuffer*>> m_uniforms;

				bool m_Updated = false;
				RHI::DescriptorSetTarget m_type = RHI::Undefined;
				RHI::DescriptorSetPipelineTarget m_pipelineType = RHI::UnlitDescriptor;

				std::vector<VkWriteDescriptorSet> t_descriptorWrites{};

				VkDescriptorSetLayout ChooseLayout(std::vector<VulkanSetLayout> a_layouts, RHI::DescriptorSetTarget a_type);
				void CreatePool(VkDevice a_logicalDevice, uint32_t a_count, std::vector<VkDescriptorType> a_types = std::vector<VkDescriptorType>(0));
				void CreateDescriptorSets(VkDevice a_logicalDevice, VkDescriptorSetLayout a_descriptorSetLayout, uint32_t a_setCount, std::vector<uint32_t> a_subSetCount);

				void CreateBuffers(uint32_t a_uniformCount, std::vector<uint32_t> a_subUniformCount);
				void DestroyBuffers();
			};
		}
	}
}


#endif