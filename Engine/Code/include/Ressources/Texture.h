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

		class Renderer;
	}

	namespace Resource
	{
		class Texture : public IResource
		{
		public:
			ENGINE_API void Create(std::string a_path);
			ENGINE_API void Destroy();

			ENGINE_API void Load(Core::Renderer* a_renderer);
			ENGINE_API void Unload(Core::Renderer* a_renderer);

			ENGINE_API int GetWidth() const { return m_width; }
			ENGINE_API int GetHeight() const { return m_height; }
			ENGINE_API unsigned char* GetData() const { if (m_data) { return m_data; } LOG_ERROR("No image data."); return nullptr; }

			ENGINE_API Core::RHI::IImage* GetImage() const { return m_image; }

		private:
			std::string m_path;
			bool m_isLoaded = false;

			int m_width = 0;
			int m_height = 0;
			unsigned char* m_data = nullptr;

			Core::RHI::IImage* m_image = nullptr;
		};
	}
}

#endif