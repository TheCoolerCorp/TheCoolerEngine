#ifndef IIMAGE_H
#define IIMAGE_H

#include "EngineExport.h"
#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanImage;
		}

		namespace RHI
		{
			class ENGINE_API IImage
			{
			public:
				virtual ~IImage() = default;
				virtual GraphicsAPI::VulkanImage* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				virtual void Create() = 0;
				virtual void CreateDepth() = 0;
				virtual void CreateTexture() = 0;
				virtual void Destroy() = 0;
			};
		}
	}
}

#endif