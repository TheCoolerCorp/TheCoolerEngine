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

#include "../../../../libs/assimp/contrib/rapidjson/include/rapidjson/rapidjson.h"


namespace Utils
{
	std::vector<char> ENGINE_API ReadFile(const std::string& a_path);
	int ENGINE_API GenerateRandomInt(const int& a_min, const int& a_max);
	
}


namespace Engine
{
	// SHARED PTR
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// UNIQUE PTR
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}
#endif