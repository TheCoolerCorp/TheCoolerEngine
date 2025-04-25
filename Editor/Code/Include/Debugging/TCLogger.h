#ifndef TCLOGGER_H
#define TCLOGGER_H

#include <mutex>
#include <fstream>
#include <vector>

#include "TCLogLevel.h"

#define TCLOG(level, message) Editor::Debugging::TCLogger::Get().TCLog(message, level, __FILE__, __LINE__)
#define TCLOG_DEBUG(message) TCLOG(Editor::Debugging::LogLevel::DEBUGLOG, message)
#define TCLOG_VERBOSE(message) TCLOG(Editor::Debugging::LogLevel::VERBOSE, message)
#define TCLOG_INFO(message) TCLOG(Editor::Debugging::LogLevel::INFO, message)
#define TCLOG_WARNING(message) TCLOG(Editor::Debugging::LogLevel::WARNING, message)
#define TCLOG_ERROR(message) TCLOG(Editor::Debugging::LogLevel::ERROR, message)
#define TCLOG_CRITICAL(message) TCLOG(Editor::Debugging::LogLevel::CRITICAL, message)

namespace Editor
{
	namespace Debugging
	{
		struct LogInfo
		{
			LogLevel mLogLevel;
			std::tm mLocalTime;
			const char* mLocation;
			int mLine;
			std::string mMessage;
		};

		class TCLogger
		{
		public:
			TCLogger(const TCLogger&) = delete;
			TCLogger& operator=(const TCLogger&) = delete;

			static TCLogger& Get()
			{
				static TCLogger s_logger{};
				return s_logger;
			}

			template <typename T>
			void TCLog(const T& a_message, LogLevel a_level, const char* a_file, int a_line);

			std::vector<LogInfo>& GetPendingLogs() { return m_pendingLogs; }

		private:
			TCLogger() = default;
			~TCLogger();
			void PrintLogFile(const std::string& a_message, const std::string& a_logLevel, const char* a_file, int a_line, const std::tm& a_localTime);
			void AddLog(const std::string& a_message, LogLevel a_logLevel, const char* a_file, int a_line, const std::tm& a_localTime);

			std::string m_logFolder = "Logs/";
			std::mutex m_logMutex;
			std::ofstream m_logFile;
			std::vector<LogInfo> m_pendingLogs{};
		};
	}
}

#include "TCLogger.inl"

#endif