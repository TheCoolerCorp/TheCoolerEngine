#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include <string>

namespace Debug
{
	enum class LogLevel : int
	{
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL
	};

	inline std::string ToString(LogLevel a_level)
	{
		switch (a_level)
		{
		case LogLevel::DEBUG: 
			return "DEBUG";
		case LogLevel::INFO: 
			return "INFO";
		case LogLevel::WARNING: 
			return "WARNING";
		case LogLevel::ERROR: 
			return "ERROR";
		case LogLevel::CRITICAL: 
			return "CRITICAL";
		default: 
			return "unknown";
		}
	}
}

#endif