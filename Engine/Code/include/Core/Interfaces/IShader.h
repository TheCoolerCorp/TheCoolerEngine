#ifndef ISHADER_H
#define ISHADER_H

#include "EngineExport.h"

#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanShader;
		}

		namespace RHI
		{
			class ILogicalDevice;

			class IShader
			{
			public:
				ENGINE_API virtual ~IShader() = default;
				ENGINE_API virtual GraphicsAPI::VulkanShader* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanPhysicalDevice!"); return nullptr; }

				ENGINE_API virtual void Create(std::string& a_path, ILogicalDevice* a_logicalDevice) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
			};
		}
	}
}

#endif
