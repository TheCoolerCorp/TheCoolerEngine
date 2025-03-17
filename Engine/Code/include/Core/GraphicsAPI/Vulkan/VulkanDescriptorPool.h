#ifndef IVULKANDESCRIPTORPOOL_h
#define IVULKANDESCRIPTORPOOL_h

#include "EngineExport.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"

#include "Core/Interfaces/IDescriptorPool.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanDescriptorPool : public RHI::IDescriptorPool
			{
			public:
				ENGINE_API ~VulkanDescriptorPool() override = default;
				ENGINE_API VulkanDescriptorPool* CastVulkan() override { return this; }

				ENGINE_API void Create(RHI::ILogicalDevice* a_logicalDevice, int a_maxFrameInFlight) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

			private:
				int m_maxFrame = 0;
				VkDescriptorPool m_pool = VK_NULL_HANDLE;
			};
		}
	}
}

#endif