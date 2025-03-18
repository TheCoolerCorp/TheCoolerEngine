#ifndef IOBJECTDESCRIPTOR_H
#define IOBJECTDESCRIPTOR_H

#include "EngineExport.h"

#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanObjectDescriptor;
		}
		namespace RHI
		{
			class ILogicalDevice;
			class IDescriptorPool;
			class IGraphicPipeline;
			class IBuffer;
			class IImage;

			class IObjectDescriptor
			{
			public:
				ENGINE_API virtual ~IObjectDescriptor() = default;
				ENGINE_API virtual GraphicsAPI::VulkanObjectDescriptor* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(ILogicalDevice* device, IGraphicPipeline* descriptorSetLayout, IDescriptorPool* descriptorPool, std::vector<IBuffer*> uniformBuffers, IImage* texture) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif
