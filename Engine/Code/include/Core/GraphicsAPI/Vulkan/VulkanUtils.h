#ifndef VULKANUTILS_H
#define VULKANUTILS_H

#include "EngineExport.h"

#include <cstring>
#include <iostream>

#include  "vulkan/vulkan.h"

namespace Engine::Core::GraphicsAPI
{
#ifdef TCDEBUG
#ifdef _MSC_VER
#define BREAKPOINT() __debugbreak()
#else
#define BREAKPOINT() __builtin_trap()
#endif
#define VK_CHECK(func, message) \
		{ \
			VkResult result = func;\
			if (result != VK_SUCCESS) { \
				std::cerr << result << "Assertion failed: " << message << " in file " << (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) << " at line " << __LINE__ << '\n'; \
				BREAKPOINT(); \
			} \
		} 
#else
#define VK_CHECK(func, message) \
		{ \
			VkResult result = func;\
			if (result != VK_SUCCESS) { \
				std::cerr << result << "Assertion failed: " << (message) << " in file " << (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) << " at line " << __LINE__ << '\n'; \
				abort(); \
			} \
		} 
#endif
}

#endif
