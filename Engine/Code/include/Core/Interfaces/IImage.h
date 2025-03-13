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
			class IPhysicalDevice;
			class ILogicalDevice;
			class ICommandPool;

			enum class ImageType
			{
				UNDEFINED = 0,
				TEXTURE = 1,
				DEPTH = 2,
			};

			class ENGINE_API IImage
			{
			public:
				virtual ~IImage() = default;
				virtual GraphicsAPI::VulkanImage* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				virtual void Create(int a_width, int a_height, ImageType a_type, IPhysicalDevice* a_physicalDevice, ILogicalDevice* a_logicalDevice, ICommandPool* a_commandPool) = 0;
				virtual void CreateDepth() = 0;
				virtual void CreateTexture() = 0;
				virtual void Destroy() = 0;
			};
		}
	}
}

#endif