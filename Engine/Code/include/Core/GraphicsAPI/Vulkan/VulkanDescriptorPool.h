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
			class ENGINE_API VulkanDescriptorPool : public RHI::IDescriptorPool
			{
			public:
				~VulkanDescriptorPool() override = default;
				GraphicsAPI::VulkanDescriptorPool* CastVulkan() override { return this; }

				void Create(RHI::ILogicalDevice* a_logicalDevice, int a_maxFrameInFlight) override;
				void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				VkDescriptorPool GetDescriptorPool() const { return m_pool; }
			private:
				int m_maxFrame = 0;
				VkDescriptorPool m_pool = VK_NULL_HANDLE;
			};
		}
	}
}

#endif