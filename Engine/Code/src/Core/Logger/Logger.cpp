#include "Core/Logger/Logger.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <mutex>

namespace Engine
{
	namespace Core
	{
		namespace Debugging
		{
			void Logger::PrintConsoleLog(const std::string& a_message, const std::string& a_logLevel,
				const std::string& a_color, const char* a_file, const int a_line, const std::tm& a_localTime)
			{
				std::cout << a_color << a_logLevel << ColorMap.at(LogColor::RESET) << " : ";
				std::cout << "[" << std::put_time(&a_localTime, "%Y-%m-%d %H:%M:%S") << "]";

				if (a_file != nullptr)
				{
					std::cout << " [" << a_file << ":" << a_line << "]";
				}

				std::cout << a_color << " " << a_message << ColorMap.at(LogColor::RESET) << '\n' << std::flush;
			}

			void Logger::PrintFileLog(const std::string& a_message, const std::string& a_logLevel, const char* a_file,
			                          const int a_line, const std::tm& a_localTime)
			{
				if (!m_logFile.is_open())
				{
					return;
				}

				m_logFile << a_logLevel << " : ";

				m_logFile << "[" << std::put_time(&a_localTime, "%Y-%m-%d %H:%M:%S") << "]";

				if (a_file != nullptr)
				{
					m_logFile << " [" << a_file << ":" << a_line << "]";
				}

				m_logFile << " " << a_message << '\n';
			}

			Logger::Logger()
			{
				std::string t_logFolder = "../../Logs";

				if (!std::filesystem::exists(t_logFolder) && !std::filesystem::is_directory(t_logFolder)) 
				{
					std::filesystem::create_directories(t_logFolder);
				}
				m_logFile.open(t_logFolder + "/TheCoolerEngine.log", std::ios::out | std::ios::app | std::ios::ate);
			}
			Logger::~Logger()
			{
				if (m_logFile.is_open())
				{
					m_logFile.close();
				}
			}

			Logger& Logger::Get()
			{
				static Logger s_logger{};
				return s_logger;
			}
		}
	}
}
