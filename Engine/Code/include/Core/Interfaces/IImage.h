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
			class VulkanImage;
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
				DEPTH = 2
			};

			struct ImageData
			{
				int mWidth = 0;
				int mHeight = 0;
				unsigned char* data = nullptr;
			};

			class IImage
			{
			public:
				ENGINE_API virtual ~IImage() = default;
				ENGINE_API virtual GraphicsAPI::VulkanImage* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(ImageType a_type, ImageData a_data, IPhysicalDevice* a_physicalDevice, ILogicalDevice* a_logicalDevice, ICommandPool* a_commandPool) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif