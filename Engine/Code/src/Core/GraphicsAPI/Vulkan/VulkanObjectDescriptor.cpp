#include "Core/GraphicsAPI/Vulkan/VulkanObjectDescritptor.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			ENGINE_API void VulkanObjectDescriptor::Create(RHI::ILogicalDevice* device, RHI::IGraphicPipeline* descriptorSetLayout, RHI::IDescriptorPool* descriptorPool, std::vector<RHI::IBuffer*> uniformBuffers, RHI::IImage* texture)
			{

			}

			ENGINE_API void VulkanObjectDescriptor::Destroy(RHI::ILogicalDevice* a_logicalDevice)
			{

			}
		}
	}
}