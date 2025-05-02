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

			ENGINE_API void Load() override;
			ENGINE_API void CreateImage(Core::Renderer* a_renderer); // Creates the image, don't forget to check yourself if it's not already created/creating
			ENGINE_API void CreateImageAsync(Core::Renderer* a_renderer);
			ENGINE_API void Unload(Core::Renderer* a_renderer) override;

			ENGINE_API int GetWidth() const { return m_width; }
			ENGINE_API int GetHeight() const { return m_height; }
			ENGINE_API unsigned char* GetData() const { if (m_data) { return m_data; } LOG_ERROR("No image data."); return nullptr; }

			[[nodiscard]] ENGINE_API bool IsCreated() const { return m_isCreated.load(std::memory_order_acquire); }
			[[nodiscard]] ENGINE_API bool IsCreating() const { return m_isCreating.load(std::memory_order_acquire); }

			[[nodiscard]] ENGINE_API Core::RHI::IImage* GetImage() const { return m_image; }
			[[nodiscard]] ENGINE_API const std::string& GetPath() const { return m_path; }
			[[nodiscard]] ENGINE_API Math::vec2 GetSize() const { return Math::vec2(static_cast<float>(m_width), static_cast<float>(m_height)); }
		private:
			std::string m_path;

			std::atomic<bool> m_isCreated = false;
			std::atomic<bool> m_isCreating = false;

			int m_width = 0;
			int m_height = 0;
			unsigned char* m_data = nullptr;

			Core::RHI::IImage* m_image = nullptr;
		};
	}
}

#endif