#include <filesystem>
#include <iomanip>
#include <fstream>
#include <mutex>
#include <string>

namespace Engine::Core::Debugging
{
	inline ENGINE_API std::string ToString(const std::string& a_message)
	{
		return a_message;
	}

	inline ENGINE_API std::string ToString(const char* a_message)
	{
		return a_message;
	}

	inline ENGINE_API std::string ToString(const int& a_message)
	{
		return std::to_string(a_message);
	}

	inline ENGINE_API std::string ToString(const float& a_message)
	{
		return std::to_string(a_message);
	}

	inline ENGINE_API std::string ToString(const double& a_message)
	{
		return std::to_string(a_message);
	}

	inline ENGINE_API std::string ToString(const bool& a_message)
	{
		return a_message ? "true" : "false";
	}

	struct Logger::MutexStruct
	{
		std::mutex m_logMutex;
	};

	struct Logger::LogFileStruct
	{
		std::ofstream m_logFile;
	};

	template <typename T>
	void Logger::Log(const T& a_message, const LogLevel a_level, const std::string& a_color, const char* a_file, const int a_line) const
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

		std::string t_message = ToString(a_message);

		PrintConsoleLog(t_message, t_logLevel, a_color, a_file, a_line, t_localTime);
		PrintFileLog(t_message, t_logLevel, a_file, a_line, t_localTime);
	}
}