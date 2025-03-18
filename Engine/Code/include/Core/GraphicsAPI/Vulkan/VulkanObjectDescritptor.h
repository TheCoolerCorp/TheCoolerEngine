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

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IGraphicPipeline* a_pipeline, RHI::IDescriptorPool* a_descriptorPool, GamePlay::GameObject* a_gameObject, int a_size) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

			private:
				std::vector<VulkanBuffer*> m_uniforms;
				std::vector<VkDescriptorSet> m_descriptorSets;
			};
		}
	}
}

#endif