#include "Core/Utils.h"


namespace Utils
{
	std::vector<char> ReadFile(const std::string& path)
	{
	    std::ifstream file(path, std::ios::ate | std::ios::binary);

	    if (!file.is_open()) 
	    {
	        LOG_ERROR("Failed to open file : " + path);
	    }

	    size_t fileSize = (size_t)file.tellg();
	    std::vector<char> buffer(fileSize);
	    file.seekg(0);
	    file.read(buffer.data(), fileSize);
	    file.close();

	    return buffer;
	}
}