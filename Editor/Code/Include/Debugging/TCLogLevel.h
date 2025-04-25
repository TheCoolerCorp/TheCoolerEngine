#ifndef TCLOGLEVEL_H
#define TCLOGLEVEL_H

#include <string>

namespace Editor
{
	namespace Debugging
	{
		enum class LogLevel : uint8_t
		{
			DEBUGLOG,
			VERBOSE,
			INFO,
			WARNING,
			ERROR,
			CRITICAL
		};

		inline std::string ToString(const LogLevel a_level)
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

#endif