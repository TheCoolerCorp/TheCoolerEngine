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

				ENGINE_API void Create(RHI::ILogicalDevice* device, RHI::IGraphicPipeline* descriptorSetLayout, RHI::IDescriptorPool* descriptorPool, std::vector<RHI::IBuffer*> uniformBuffers, RHI::IImage* texture) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

			private:
				std::vector<VulkanBuffer*> m_uniforms;
			};
		}
	}
}

#endif