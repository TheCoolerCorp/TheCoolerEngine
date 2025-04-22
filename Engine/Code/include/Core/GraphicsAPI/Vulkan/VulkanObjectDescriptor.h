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

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IGraphicPipeline* a_graphicPipeline, RHI::DescriptorSetTarget a_type, int a_count, std::vector<RHI::DescriptorSetType> a_types = std::vector<RHI::DescriptorSetType>(0)) override;

				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API void SetTexture(RHI::ILogicalDevice* a_logicalDevice, RHI::IImage* a_image, uint32_t a_dstBinding, uint32_t a_count) override;
				ENGINE_API void SetMat(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, void* a_matData, uint32_t a_dstBinding, uint32_t a_count) override;

				ENGINE_API void UpdateUniforms(RHI::ILogicalDevice* a_logicalDevice, void* a_data, int a_imageIndex) override;


				ENGINE_API std::vector<VkDescriptorSet> GetDescriptorSets() const { return m_sets; }
				ENGINE_API RHI::DescriptorSetTarget GetType() { return m_type; }

				ENGINE_API bool IsUpdated() override { return m_Updated; }
			private:
				std::vector<VkDescriptorSet> m_sets;
				VkDescriptorPool m_pool = VK_NULL_HANDLE;

				std::vector<VulkanBuffer*> m_uniforms;

				bool m_Updated = false;
				RHI::DescriptorSetTarget m_type = RHI::UNDEFINED;

				std::vector<VkWriteDescriptorSet> t_descriptorWrites{};

				VkDescriptorSetLayout ChooseLayout(std::vector<VulkanSetLayout> a_layouts, RHI::DescriptorSetTarget a_type);
				void CreatePool(VkDevice a_logicalDevice, uint32_t a_count, std::vector<VkDescriptorType> a_types = std::vector<VkDescriptorType>(0));
				void CreateDescriptorSets(VkDevice a_logicalDevice, VkDescriptorSetLayout a_descriptorSetLayout, uint32_t a_count);

				void CreateBuffers(uint32_t a_count);
				void DestroyBuffers();
			};
		}
	}
}


#endif