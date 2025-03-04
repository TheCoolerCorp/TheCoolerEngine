#include "Logger/Logger.h"

#include <iomanip>
#include <iostream>

namespace Debug
{
	struct Logger::MutexStruct
	{
		std::mutex m_logMutex;
	};

	Logger::Logger() : m_mutexStruct(new MutexStruct) {}
	Logger::~Logger() { delete m_mutexStruct; }

	Logger& Debug::Logger::Get()
	{
		static Logger s_logger{};
		return s_logger;
	}

	void Logger::Log(const std::string& a_message, LogLevel a_level, const std::string& a_color, const char* a_file, int a_line)
	{
		std::lock_guard<std::mutex> t_lock(m_mutexStruct->m_logMutex);

		std::time_t now = std::time(nullptr);
		std::tm localTime;
		localtime_s(&localTime, &now);

		std::string t_logLevel = ToString(a_level);

		std::cout << a_color << t_logLevel << ColorMap.at(LogColor::RESET) << " : ";

		std::cout << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "]";

		if (a_file != nullptr)
		{
			std::cout << " [" << a_file << ":" << a_line << "]";
		}

		std::cout << a_color << " " << a_message << ColorMap.at(LogColor::RESET) << '\n';
	}
}
