#ifndef IRENDERPASS_H
#define IRENDERPASS_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		class Renderer;

		namespace GraphicsAPI
		{
			class VulkanRenderPassManager;
		}
		namespace RHI
		{
			class ISwapChain;
			class ILogicalDevice;
			class IPhysicalDevice;

			class IRenderPass
			{
			public:
				ENGINE_API virtual ~IRenderPass() = default;
				ENGINE_API virtual GraphicsAPI::VulkanRenderPassManager* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(Renderer* renderer) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}
#endif