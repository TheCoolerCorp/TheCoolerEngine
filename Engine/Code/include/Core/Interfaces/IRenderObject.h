#ifndef IRENDEROBJECT_H
#define IRENDEROBJECT_H

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanRenderObject;
		}
		namespace RHI
		{
			class ILogicalDevice;
			class IPhysicalDevice;
			class ICommandPool;
			class ISurface;
			class IGraphicPipeline;
			class IImage;

			class IRenderObject
			{
			public:
				ENGINE_API virtual ~IRenderObject() = default;

				ENGINE_API virtual GraphicsAPI::VulkanRenderObject* CastVulkan() { return nullptr; }

				ENGINE_API virtual void Create(ILogicalDevice* a_logicalDevice, IPhysicalDevice* a_physicalDevice, ISurface* a_surface, ICommandPool* a_commandPool, IGraphicPipeline* a_graphicPipeline, int a_maxFrame) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;

				ENGINE_API virtual void SetUniforms(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, void* a_data, int a_maxFrame) = 0;
				ENGINE_API virtual void SetTexture(RHI::ILogicalDevice* a_logicalDevice, RHI::IImage* a_image, int a_maxFrame) = 0;

				ENGINE_API virtual void UpdateUniforms(ILogicalDevice* a_logicalDevice, void* a_data, int a_imageIndex) = 0;

			};
		}
	}
}


#endif