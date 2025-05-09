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
				DEPTH = 2,
				CUBEMAP = 3
			};


			/*
			 * Albedo = RBGA SRGB
			 * Normal, roughness, metallic, ao = R8 UNORM
			 */
			enum class ImageFormat : uint8_t
			{
				FORMAT_UNDEFINED = 0,

				FORMAT_R8G8B8_SRBG = 29,
				FORMAT_R8G8B8A8_SRBG = 43,

				FORMAT_R8G8B8A8_UNORM = 37,
				FORMAT_R8G8B8_UNORM = 28,
				FORMAT_R8_UNORM = 9,

				FORMAT_D32_SFLOAT = 126,
				FORMAT_D32_SFLOAT_S8_UINT = 130
			};

			struct ImageData
			{
				int mWidth = 0;
				int mHeight = 0;
				unsigned char* data = nullptr;
				int channels = 0;
			};

			class IImage
			{
			public:
				ENGINE_API virtual ~IImage() = default;
				ENGINE_API virtual GraphicsAPI::VulkanImage* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(ImageType a_type, ImageFormat a_format, ImageData a_data, IPhysicalDevice* a_physicalDevice, ILogicalDevice* a_logicalDevice, ICommandPool* a_commandPool) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif