#ifndef ICAMERADESCRIPTOR_H
#define ICAMERADESCRIPTOR_H

#include "Math/mat4.h"

#include "EngineExport.h"

#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanCameraDescriptor;
		}
		namespace RHI
		{
			class ILogicalDevice;
			class IPhysicalDevice;
			class IDescriptorPool;
			class IGraphicPipeline;
			class ICommandPool;
			class IBuffer;

			class ICameraDescriptor
			{
			public:
				ENGINE_API virtual ~ICameraDescriptor() = default;
				ENGINE_API virtual GraphicsAPI::VulkanCameraDescriptor* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanObjectDescriptor!"); return nullptr; }

				ENGINE_API virtual void Create(ILogicalDevice* a_logicalDevice, IPhysicalDevice* a_physicalDevice, IGraphicPipeline* a_pipeline, IDescriptorPool* a_descriptorPool, ICommandPool* a_commandPool, Math::mat4 a_vp) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
				ENGINE_API virtual void Update(RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData) = 0;
			};
		}
	}
}

#endif