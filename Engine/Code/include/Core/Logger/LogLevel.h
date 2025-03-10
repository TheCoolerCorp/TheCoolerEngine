#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include <string>

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace Debugging
		{
			enum class LogLevel : int
			{
				DEBUGLOG,
				VERBOSE,
				INFO,
				WARNING,
				ERROR,
				CRITICAL
			};

			inline std::string ENGINE_API ToString(const LogLevel a_level)
			{
				switch (a_level)
				{
				case LogLevel::DEBUGLOG:
					return "DEBUG";
				case LogLevel::VERBOSE:
					return "VERBOSE";
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
	}
}

#endif