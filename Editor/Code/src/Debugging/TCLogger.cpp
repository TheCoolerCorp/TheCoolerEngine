#include "Debugging/TCLogger.h"

#include <filesystem>

namespace Editor
{
	namespace Debugging
	{
		TCLogger::~TCLogger()
		{
			if (m_logFile.is_open())
			{
				m_logFile.close();
			}
			m_pendingLogs.clear();
		}

		void TCLogger::PrintLogFile(const std::string& a_message, const std::string& a_logLevel, const char* a_file,
			const int a_line, const std::tm& a_localTime)
		{
			if (!m_logFile.is_open())
			{
				if (!std::filesystem::exists(m_logFolder) && !std::filesystem::is_directory(m_logFolder))
				{
					std::filesystem::create_directories(m_logFolder);
				}
				m_logFile.open(m_logFolder + "Log.log", std::ios::out | std::ios::trunc);
			}

			m_logFile << a_logLevel << " : ";

			m_logFile << "[" << std::put_time(&a_localTime, "%Y-%m-%d %H:%M:%S") << "]";

			if (a_file != nullptr)
			{
				m_logFile << " [" << a_file << ":" << a_line << "]";
			}

			m_logFile << " " << a_message << '\n';
		}

		void TCLogger::AddLog(const std::string& a_message, const LogLevel a_logLevel, const char* a_file,
			const int a_line, const std::tm& a_localTime)
		{
			LogInfo t_logInfo{
				.mLogLevel= a_logLevel,
				.mLocalTime= a_localTime,
				.mLocation= a_file,
				.mLine= a_line,
				.mMessage= a_message
			};

			m_pendingLogs.emplace_back(t_logInfo);
		}
	}
}
