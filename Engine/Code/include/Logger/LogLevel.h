#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include <string>

#include "EngineExport.h"

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

	inline std::string ENGINE_API ToString(const LogLevel a_level)
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