#ifndef VULKANCAMERADESCRIPTOR_H
#define VULKANCAMERADESCRIPTOR_H

#include <vulkan/vulkan.h>

#include "Core/Interfaces/ICameraDescriptor.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanBuffer;

			class VulkanCameraDescriptor : public RHI::ICameraDescriptor
			{
			public:
				VulkanCameraDescriptor* CastVulkan() override { return this; }
				void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool, RHI::ICommandPool* a_commandPool, Math::mat4 a_vp, int a_size) override;
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				void Update(uint32_t a_frameIndex, RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData) override;

			private:
				std::vector<VulkanBuffer*> m_uniforms;
				std::vector<VkDescriptorSet> m_descriptorSets;
			};
		}
	}
}

#endif