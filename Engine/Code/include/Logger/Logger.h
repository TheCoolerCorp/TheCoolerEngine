#ifndef LOGGER_H
#define LOGGER_H

#include "LogLevel.h"

namespace Debug
{
	class Logger
	{
	public:
		void Log(const std::string& a_message, LogLevel a_level, const char* a_file, int a_line);
#define LOG(Logger, level, message) (Logger).Log(message, level, __FILE__, __LINE__)
#define LOG_DEBUG(Logger, message) LOG(Logger, logtard::LogLevel::DEBUG, message)
#define LOG_INFO(Logger, message) LOG(Logger, logtard::LogLevel::INFO, message)
#define LOG_WARNING(Logger, message) LOG(Logger, logtard::LogLevel::WARNING, message)
#define LOG_ERROR(Logger, message) LOG(Logger, logtard::LogLevel::ERROR, message)
#define LOG_CRITICAL(Logger, message) LOG(Logger, logtard::LogLevel::CRITICAL, message)
	};
}

#endif