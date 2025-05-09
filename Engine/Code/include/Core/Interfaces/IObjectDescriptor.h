#ifndef IOBJECTDESCRIPTOR_H
#define IOBJECTDESCRIPTOR_H

#include <cstdint>
#include <vector>

#include "EngineExport.h"

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
			class IPhysicalDevice;
			class ICommandPool;
			class ISurface;
			class IGraphicPipeline;
			class IImage;

			enum DescriptorSetTarget : int
			{
				Camera = 0,
				Object = 1,
				Lights = 2,
				Undefined = -1
			};

			enum DescriptorSetPipelineTarget : int
			{
				UnlitDescriptor,
				LitDescriptor,
				SkyBoxDescriptor
			};

			enum DescriptorSetDataType : int
			{
				DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER = 1,
				DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER = 6
			};

			enum ObjectType : int
			{
				RenderObject,
				DataObject
			};

			class IObjectDescriptor
			{
			public:
				ENGINE_API virtual ~IObjectDescriptor() = default;

				ENGINE_API virtual GraphicsAPI::VulkanObjectDescriptor* CastVulkan() { return nullptr; }

				ENGINE_API virtual void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IGraphicPipeline* a_graphicPipeline, RHI::DescriptorSetTarget a_type, uint32_t a_setCount, uint32_t a_uniformCount, std::vector<uint32_t> a_subUniformCount, std::vector<RHI::DescriptorSetDataType> a_types) = 0;

				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;

				ENGINE_API virtual void SetTexture(RHI::ILogicalDevice* a_logicalDevice, RHI::IImage* a_image, uint32_t a_dstBinding, uint32_t a_count) = 0;
				ENGINE_API virtual void SetUniform(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, uint32_t a_bufferIndex, void* a_data, uint32_t a_dataSize, uint32_t a_dstBinding, uint32_t a_descriptorCount) = 0;

				ENGINE_API virtual void UpdateUniforms(RHI::ILogicalDevice* a_logicalDevice, uint32_t a_bufferIndex, void* a_data, uint32_t a_dataSize, int a_imageIndex) = 0;

				ENGINE_API virtual bool IsUpdated() { return false; }

				ENGINE_API virtual DescriptorSetPipelineTarget GetPipelineTargetType() = 0;

			};
		}
	}
}


#endif