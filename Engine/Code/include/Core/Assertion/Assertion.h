#ifndef ASSERTION_H
#define ASSERTION_H

#include <cstring>
#include "EngineExport.h"

namespace Engine::Core::Debugging
{
#ifdef TCDEBUG
#ifdef _MSC_VER
#define BREAKPOINT() __debugbreak()
#else
#define BREAKPOINT() __builtin_trap()
#endif
#define ASSERT(condition, message) \
		do { \
			if (!(condition)) { \
				std::cerr << "Assertion failed: " << message << " in file " << (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) << " at line " << __LINE__ << std::endl; \
				BREAKPOINT(); \
			} \
		} while(0)
#else
#define ASSERT(condition, message) \
		do { \
			if (!(condition)) { \
				std::cerr << "Assertion failed: " << message << " in file " << (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) << " at line " << __LINE__ << std::endl; \
				abort(); \
			} \
		} while(0)
#endif
}

#endif