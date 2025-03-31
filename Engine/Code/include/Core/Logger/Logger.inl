#include <filesystem>
#include <iomanip>
#include <string>

namespace Engine
{
	namespace Core
	{
		namespace Debugging
		{
			template <typename T>
			std::string ToString(const T& a_value)
			{
				std::ostringstream t_oss;
				t_oss << a_value;
				return t_oss.str();
			}

			/*template <typename T>
			void Logger::Log(const T& a_message, LogLevel a_level, const LogColor& a_color, const char* a_file, int a_line);
			{
				std::lock_guard<std::mutex> t_lock(m_logMutex);

				std::time_t t_now = std::time(nullptr);
				std::tm t_localTime;
				localtime_s(&t_localTime, &t_now);

				std::string t_logLevel = ToString(a_level);

				std::string t_message = ToString(a_message);

				PrintConsoleLog(t_message, t_logLevel, a_color, a_file, a_line, t_localTime);
				PrintFileLog(t_message, t_logLevel, a_file, a_line, t_localTime);
			}*/

			template <typename T>
			void Logger::Log(const T& a_message, LogLevel a_level, const LogColor& a_color, const char* a_file, int a_line)
			{
				std::lock_guard<std::mutex> t_lock(m_logMutex);

				std::time_t t_now = std::time(nullptr);
				std::tm t_localTime;
				localtime_s(&t_localTime, &t_now);

				std::string t_logLevel = ToString(a_level);

				std::string t_message = ToString(a_message);

				PrintConsoleLog(t_message, t_logLevel, a_color, a_file, a_line, t_localTime);
				PrintFileLog(t_message, t_logLevel, a_file, a_line, t_localTime);
			}
		}
	}
}