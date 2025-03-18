#include  "GamePlay/Component.h"

namespace Engine
{
	namespace GamePlay
	{
		void Component::ComputeName(std::string a_path)
		{
			std::filesystem::path filePath(a_path);

			std::string filename = filePath.stem().string();

			std::string extension = filePath.extension().string();
			if (!extension.empty() && extension[0] == '.')
			{
				extension = extension.substr(1);
			}

			m_name = filename + "_" + extension;
		}

	}
}