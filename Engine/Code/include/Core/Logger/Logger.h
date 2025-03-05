#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>

#include "LogLevel.h"
#include "LogColor.h"

#define LOG(level, message, color) Engine::Core::Debugging::Logger::Get().Log(message, level, color, std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__)
#define LOG_DEBUG(message) LOG(Engine::Core::Debugging::LogLevel::DEBUGLOG, message, Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::BOLD_BLUE))
#define LOG_INFO(message) LOG(Engine::Core::Debugging::LogLevel::INFO, message, Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::BOLD_GREEN))
#define LOG_WARNING(message) LOG(Engine::Core::Debugging::LogLevel::WARNING, message, Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::BOLD_YELLOW))
#define LOG_ERROR(message) LOG(Engine::Core::Debugging::LogLevel::ERROR, message, Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::BOLD_RED))
#define LOG_CRITICAL(message) LOG(Engine::Core::Debugging::LogLevel::CRITICAL, message, Engine::Core::Debugging::ColorMap.at(Engine::Core::Debugging::LogColor::BOLD_MAGENTA))

namespace Engine
{
	namespace Core
	{
		namespace Debugging
		{
			class ENGINE_API Logger
			{
			public:
				static Logger& Get();

				template <typename T>
				void Log(const T& a_message, LogLevel a_level, const std::string& a_color, const char* a_file, int a_line) const;

			private:
				struct MutexStruct;
				MutexStruct* m_mutexStruct;
				struct LogFileStruct;
				LogFileStruct* m_logFileStruct;
				Logger();
				~Logger();
				Logger(const Logger&) = delete;
				Logger& operator=(const Logger&) = delete;
				static void PrintConsoleLog(const std::string& a_message, const std::string& a_logLevel, const std::string& a_color, const char* a_file, int a_line, const std::tm& a_localTime);
				void PrintFileLog(const std::string& a_message, const std::string& a_logLevel, const char* a_file, int a_line, const std::tm& a_localTime) const;
			};

		}
	}
}

#include "Logger.tpp"

#endif