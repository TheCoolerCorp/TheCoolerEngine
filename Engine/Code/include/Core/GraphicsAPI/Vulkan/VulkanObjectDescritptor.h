#ifndef VULKANOBJECTDESCRIPTOR_H
#define VULKANOBJECTDESCRIPTOR_H

#include "EngineExport.h"

#include <vector>

#include "Core/Interfaces/IObjectDescriptor.h"
#include "VulkanBuffer.h"

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

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool, RHI::ICommandPool* a_commandPool, GamePlay::GameObject* a_gameObject, int a_size) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;
				ENGINE_API void Update(uint32_t a_frameIndex, RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData) override;

				ENGINE_API const VkDescriptorSet* GetDescriptorSets() const { return m_descriptorSets.data(); }
				ENGINE_API const uint32_t GetDescriptorSetsCount() const { return static_cast<uint32_t>(m_descriptorSets.size()); }

			private:
				std::vector<VulkanBuffer*> m_uniforms;
				std::vector<VkDescriptorSet> m_descriptorSets;
			};
		}
	}
}

#endif