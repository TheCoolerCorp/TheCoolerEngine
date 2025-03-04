#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>

#include "LogLevel.h"
#include "LogColor.h"

namespace Debug
{
	class ENGINE_API Logger
	{
	public:
		static Logger& Get();
		void Log(const std::string& a_message, LogLevel a_level, const std::string& a_color,const char* a_file, int a_line);

	private:
		struct MutexStruct;
		MutexStruct* m_mutexStruct;
		Logger();
		~Logger();
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
	};

#define LOG(level, message, color) Logger::Get().Log(message, level, color, (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__), __LINE__)
#define LOG_DEBUG(message) LOG(LogLevel::DEBUG, message, ColorMap.at(LogColor::BOLD_BLUE))
#define LOG_INFO(message) LOG(LogLevel::INFO, message, ColorMap.at(LogColor::BOLD_GREEN))
#define LOG_WARNING(message) LOG(LogLevel::WARNING, message, ColorMap.at(LogColor::BOLD_YELLOW))
#define LOG_ERROR(message) LOG(LogLevel::ERROR, message, ColorMap.at(LogColor::BOLD_RED))
#define LOG_CRITICAL(message) LOG(LogLevel::CRITICAL, message, ColorMap.at(LogColor::BOLD_MAGENTA))
}

#endif