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
			ENGINE_API void Create(std::string a_path) override;
			ENGINE_API void Destroy() override;

			ENGINE_API void Load(Core::Renderer* a_renderer);
			ENGINE_API void Unload(Core::Renderer* a_renderer);

			ENGINE_API int GetWidth() const { return m_width; }
			ENGINE_API int GetHeight() const { return m_height; }
			ENGINE_API unsigned char* GetData() const { if (m_data) { return m_data; } LOG_ERROR("No image data."); return nullptr; }

			[[nodiscard]] ENGINE_API Core::RHI::IImage* GetImage() const { return m_image; }
			[[nodiscard]] ENGINE_API const std::string& GetPath() const { return m_path; }
			[[nodiscard]] ENGINE_API bool IsLoaded() const { return m_isLoaded; }
			[[nodiscard]] ENGINE_API Math::vec2 GetSize() const { return Math::vec2(static_cast<float>(m_width), static_cast<float>(m_height)); }
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