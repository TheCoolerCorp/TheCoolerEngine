#ifndef IOBJECTDESCRIPTOR_H
#define IOBJECTDESCRIPTOR_H

#include "EngineExport.h"

#include "Core/Interfaces/IGraphicPipeline.h"

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
				Common = 0,
				Per = 1,
				UNDEFINED = 2
			};

			

			enum DescriptorSetType : int
			{
				DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER = 1,
				DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER = 6
			};

			class IObjectDescriptor
			{
			public:
				ENGINE_API virtual ~IObjectDescriptor() = default;

				ENGINE_API virtual GraphicsAPI::VulkanObjectDescriptor* CastVulkan() { return nullptr; }

				ENGINE_API virtual void Create(ILogicalDevice* a_logicalDevice, IGraphicPipeline* a_graphicPipeline, DescriptorSetTarget a_type, int a_count, std::vector<DescriptorSetType> a_types = std::vector<DescriptorSetType>(0)) = 0;

				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;

				ENGINE_API virtual void SetTexture(RHI::ILogicalDevice* a_logicalDevice, RHI::IImage* a_image, uint32_t a_dstBinding, uint32_t a_count) = 0;
				ENGINE_API virtual void SetMat(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool,void* a_matData, uint32_t a_dstBinding, uint32_t a_count) = 0;

				ENGINE_API virtual void UpdateUniforms(ILogicalDevice* a_logicalDevice, void* a_data, int a_imageIndex) = 0;

				ENGINE_API virtual bool IsUpdated() { return false; }

			};
		}
	}
}


#endif