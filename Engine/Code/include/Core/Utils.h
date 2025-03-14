#ifndef UTILS_H
#define UTILS_H

#include "EngineExport.h"
#include  "Core/Logger/Logger.h"

#include <fstream>
#include <vector>

namespace Utils
{
	std::vector<char> ENGINE_API ReadFile(const std::string& path);
	
} 
#endif