#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"

#include "EngineExport.h"
#include <string>

#include "Core/Logger/Logger.h"
#include "IResource.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class IImage;
		}
	}

	namespace Resource
	{
		class Texture : public IResource
		{
		public:
			ENGINE_API void Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool) override;
			ENGINE_API void Destroy(Core::RHI::ILogicalDevice* a_logicalDevice) override;

			ENGINE_API int GetWidth() const { return m_width; }
			ENGINE_API int GetHeight() const { return m_height; }
			ENGINE_API unsigned char* GetData() const { if (m_data) { return m_data; } LOG_ERROR("No image data."); return nullptr; }

			ENGINE_API Core::RHI::IImage* GetImage() const { return m_image; }

		private:
			int m_width = 0;
			int m_height = 0;
			unsigned char* m_data = nullptr;

			Core::RHI::IImage* m_image = nullptr;
		};
	}
}

#endif