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
				const LogColor& a_color, const char* a_file, const int a_line, const std::tm& a_localTime)
			{
				const char* t_color = GetColor(a_color);

				std::cout << t_color << a_logLevel << GetColor(LogColor::RESET) << " : ";
				std::cout << "[" << std::put_time(&a_localTime, "%Y-%m-%d %H:%M:%S") << "]";

				if (a_file != nullptr)
				{
					std::cout << " [" << a_file << ":" << a_line << "]";
				}

				std::cout << t_color << " " << a_message << GetColor(LogColor::RESET) << '\n';
			}

			void Logger::PrintFileLog(const std::string& a_message, const std::string& a_logLevel, const char* a_file,
			                          const int a_line, const std::tm& a_localTime)
			{
				if (!m_logFile.is_open())
				{
					if (!std::filesystem::exists(m_logFolder) && !std::filesystem::is_directory(m_logFolder))
					{
						std::filesystem::create_directories(m_logFolder);
					}
					m_logFile.open(m_logFolder + "/TheCoolerEngine.log", std::ios::out | std::ios::trunc);
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
				m_logFolder = "../../Logs";
			}
			Logger::~Logger()
			{
				if (m_logFile.is_open())
				{
					m_logFile.close();
				}
			}
		}
	}
}
