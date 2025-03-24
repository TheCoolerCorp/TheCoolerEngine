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
				ENGINE_API VulkanCameraDescriptor* CastVulkan() override { return this; }
				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool, RHI::ICommandPool* a_commandPool, Math::mat4 a_vp) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Update(RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData) override;

				ENGINE_API VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

			private:
				VulkanBuffer* m_uniform{};
				VkDescriptorSet m_descriptorSet{};
			};
		}
	}
}

#endif