#ifndef IOBJECTDESCRIPTOR_H
#define IOBJECTDESCRIPTOR_H

#include "EngineExport.h"

#include "Core/Logger/Logger.h"
namespace Engine
{
	namespace GamePlay
	{
		class GameObject;
	}
	namespace Core
	{

		namespace GraphicsAPI
		{
			class VulkanObjectDescriptor;
		}
		namespace RHI
		{
			class ILogicalDevice;
			class IPhysicalDevice;
			class IDescriptorPool;
			class IGraphicPipeline;
			class ICommandPool;
			class IBuffer;
			class IImage;

			class IObjectDescriptor
			{
			public:
				ENGINE_API virtual ~IObjectDescriptor() = default;
				ENGINE_API virtual GraphicsAPI::VulkanObjectDescriptor* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(ILogicalDevice* a_logicalDevice, IPhysicalDevice* a_physicalDevice, IGraphicPipeline* a_pipeline, IDescriptorPool* a_descriptorPool, ICommandPool* a_commandPool, GamePlay::GameObject* a_gameObject, int a_size) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
				ENGINE_API virtual void Update(uint32_t a_frameIndex, RHI::ILogicalDevice* a_logicalDevice, void* a_uploadData) = 0;
			};
		}
	}
}

#endif
