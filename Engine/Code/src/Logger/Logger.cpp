#include "Logger/Logger.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <mutex>

namespace Debug
{
	struct Logger::MutexStruct
	{
		std::mutex m_logMutex;
	};

	struct Logger::LogFileStruct
	{
		std::ofstream m_logFile;
	};

	Logger::Logger() : m_mutexStruct(new MutexStruct), m_logFileStruct(new LogFileStruct)
	{
		/*std::string t_logFolder = "../Logs";

		if (!std::filesystem::exists(t_logFolder))
		{
			std::filesystem::create_directories(t_logFolder);
		}*/
		m_logFileStruct->m_logFile.open("TheCoolerEngine.log", std::ios::out | std::ios::app);
	}
	Logger::~Logger()
	{
		delete m_mutexStruct;
		if (m_logFileStruct->m_logFile.is_open())
		{
			m_logFileStruct->m_logFile.close();
		}
		delete m_logFileStruct;
	}

	void Logger::PrintConsoleLog(const std::string& a_message, const std::string& a_logLevel,
		const std::string& a_color, const char* a_file, const int a_line, const std::tm& a_localTime)
	{
		std::cout << a_color << a_logLevel << ColorMap.at(LogColor::RESET) << " : ";

		std::cout << "[" << std::put_time(&a_localTime, "%Y-%m-%d %H:%M:%S") << "]";

		if (a_file != nullptr)
		{
			std::cout << " [" << a_file << ":" << a_line << "]";
		}

		std::cout << a_color << " " << a_message << ColorMap.at(LogColor::RESET) << '\n';
	}

	void Logger::PrintFileLog(const std::string& a_message, const std::string& a_logLevel, const char* a_file,
		const int a_line, const std::tm& a_localTime) const
	{
		if (!m_logFileStruct)
		{
			return;
		}

		if (!m_logFileStruct->m_logFile.is_open())
		{
			return;
		}

		m_logFileStruct->m_logFile << a_logLevel << " : ";

		m_logFileStruct->m_logFile << "[" << std::put_time(&a_localTime, "%Y-%m-%d %H:%M:%S") << "]";

		if (a_file != nullptr)
		{
			m_logFileStruct->m_logFile << " [" << a_file << ":" << a_line << "]";
		}

		m_logFileStruct->m_logFile << " " << a_message << '\n';
	}

	Logger& Logger::Get()
	{
		static Logger s_logger{};
		return s_logger;
	}

	void Logger::Log(const std::string& a_message, const LogLevel a_level, const std::string& a_color, const char* a_file, const int a_line) const
	{
		if (!m_mutexStruct)
		{
			return;
		}

		std::lock_guard<std::mutex> t_lock(m_mutexStruct->m_logMutex);

		std::time_t t_now = std::time(nullptr);
		std::tm t_localTime;
		localtime_s(&t_localTime, &t_now);

		std::string t_logLevel = ToString(a_level);

		PrintConsoleLog(a_message, t_logLevel, a_color, a_file, a_line, t_localTime);
		PrintFileLog(a_message, t_logLevel, a_file, a_line, t_localTime);
	}
}
