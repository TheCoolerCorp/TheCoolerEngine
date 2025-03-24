#ifndef UTILS_H
#define UTILS_H

#include "EngineExport.h"
#include  "Core/Logger/Logger.h"

#include <fstream>
#include <vector>
#include <random>
#include <time.h>
#include <thread>
#include <string>
#include <iostream>


namespace Utils
{
	std::vector<char> ENGINE_API ReadFile(const std::string& a_path);
	int ENGINE_API GenerateRandomInt(const int& a_min, const int& a_max);
	
} 
#endif