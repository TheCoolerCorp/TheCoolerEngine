#ifndef TEXTURE_H
#define TEXTURE_H


#include "EngineExport.h"
#include <string>

#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Ressources
	{

		class Texture
		{
		public:
			struct Data
			{
				int mWidth = 0;
				int mHeight = 0;
				int mChannel = 0;
				unsigned char* mImage;
			};

			ENGINE_API Texture();
			ENGINE_API ~Texture() { delete m_data; }

			ENGINE_API void Create(std::string a_path);
			ENGINE_API void Destroy();
			Data* m_data = nullptr;


		private:

		};
	}
}

#endif