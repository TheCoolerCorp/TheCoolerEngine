#ifndef TEXTURE_H
#define TEXTURE_H


#include "EngineExport.h"
#include <string>

#include "Core/Logger/Logger.h"
#include "IResource.h"

namespace Engine
{
	namespace Resource
	{
		class Texture : public IResource
		{
		public:
			ENGINE_API void Create(std::string a_path) override;
			ENGINE_API void Destroy() override;

			ENGINE_API int GetWidth() { return m_width; }
			ENGINE_API int GetHeight() { return m_height; }
			ENGINE_API unsigned char* GetData() { if (m_data) { return m_data; } LOG_ERROR("No image data."); return nullptr; }

		private:
			int m_width = 0;
			int m_height = 0;
			unsigned char* m_data = nullptr;
		};
	}
}

#endif