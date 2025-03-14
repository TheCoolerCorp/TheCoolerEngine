#ifndef MESH_H
#define MESH_H

#include "EngineExport.h"
#include <string>

#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Ressources
	{

		class ENGINE_API Texture
		{
		public:
			struct Data
			{
				int mWidth = 0;
				int mHeight = 0;
				int mChannel = 0;
				unsigned char* mImage;
			};

			Texture();
			~Texture() { delete m_data; }

			void Create(std::string a_path);
			void Destroy();
			Data* m_data = nullptr;


		private:

		};
	}
}

#endif