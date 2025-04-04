#ifndef VULKANRENDEROBJECT_H
#define VULKANRENDEROBJECT_H

#include "EngineExport.h"

#include "Core/Interfaces/IRenderObject.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include  "Core/GraphicsAPI/Vulkan/VulkanBuffer.h"
#include "Core/GraphicsAPI/Vulkan/VulkanImage.h"

#include <vector>
#include <array>

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanRenderObject : public RHI::IRenderObject
			{
			public:
				ENGINE_API ~VulkanRenderObject() override = default;

				ENGINE_API VulkanRenderObject* CastVulkan() override { return this; }

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ISurface* a_surface, RHI::ICommandPool* a_commandPool, RHI::IGraphicPipeline* a_graphicPipeline, int a_maxFrame) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API void SetData(RHI::ILogicalDevice* a_logicalDevice, RHI::IPhysicalDevice* a_physicalDevice, RHI::ICommandPool* a_commandPool, RHI::IImage* a_image, void* a_data, int a_maxFrame) override;

				ENGINE_API void UpdateUniforms(RHI::ILogicalDevice* a_logicalDevice, void* a_data, int a_imageIndex) override;

				ENGINE_API std::vector<VkDescriptorSet> GetDescriptorSets() const { return m_sets; }

				ENGINE_API bool IsUpdated() override { return m_Updated; }
			private:
				std::vector<VkDescriptorSet> m_sets;
				VkDescriptorPool m_pool = VK_NULL_HANDLE;
				std::vector<VulkanBuffer*> m_uniforms;
				bool m_Updated = false;

				void CreatePool(VkDevice a_logicalDevice, int a_maxFrame);
				void CreateDescriptorSets(VkDevice a_logicalDevice, VkDescriptorSetLayout a_descriptorSetLayout, int a_maxFrame);
	
			};
		}
	}
}


#endif