#include "Core/Utils.h"


namespace Utils
{
	std::vector<char> ReadFile(const std::string& a_path)
	{
	    std::ifstream file(a_path, std::ios::ate | std::ios::binary);

	    if (!file.is_open()) 
	    {
	        LOG_ERROR("Failed to open file : " + a_path);
	    }

	    size_t fileSize = (size_t)file.tellg();
	    std::vector<char> buffer(fileSize);
	    file.seekg(0);
	    file.read(buffer.data(), fileSize);
	    file.close();

	    return buffer;
	}

	int GenerateRandomInt(const int& a_min, const int& a_max)
	{
		static thread_local std::mt19937 generator(std::random_device{}());
		std::uniform_int_distribution<int> distribution(a_min, a_max);
		return distribution(generator);
	}
}