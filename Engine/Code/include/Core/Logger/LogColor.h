#ifndef LOGCOLOR_H
#define LOGCOLOR_H

#include <array>

namespace Engine
{
    namespace Core
    {
	    namespace Debugging
	    {
            enum class LogColor : uint8_t
            {
                RESET,
                BLACK,
                RED,
                GREEN,
                YELLOW,
                BLUE,
                MAGENTA,
                CYAN,
                WHITE,
                GRAY,
                BOLD_BLACK,
                BOLD_RED,
                BOLD_GREEN,
                BOLD_YELLOW,
                BOLD_BLUE,
                BOLD_MAGENTA,
                BOLD_CYAN,
                BOLD_WHITE,
            };

            /*static const std::unordered_map<LogColor, std::string> ColorMap =
            {
                {LogColor::RESET, "\033[0m"},
                {LogColor::BLACK, "\033[30m"},
                {LogColor::RED, "\033[31m"},
                {LogColor::GREEN, "\033[32m"},
                {LogColor::YELLOW, "\033[33m"},
                {LogColor::BLUE, "\033[34m"},
                {LogColor::MAGENTA, "\033[35m"},
                {LogColor::CYAN, "\033[36m"},
                {LogColor::WHITE, "\033[0m"},
                {LogColor::GRAY, "\033[37m"},
                {LogColor::BOLD_BLACK, "\033[1m\033[90m"},
                {LogColor::BOLD_RED, "\033[1m\033[91m"},
                {LogColor::BOLD_GREEN, "\033[1m\033[92m"},
                {LogColor::BOLD_YELLOW, "\033[1m\033[93m"},
                {LogColor::BOLD_BLUE, "\033[1m\033[94m"},
                {LogColor::BOLD_MAGENTA, "\033[1m\033[95m"},
                {LogColor::BOLD_CYAN, "\033[1m\033[96m"},
                {LogColor::BOLD_WHITE, "\033[1m\033[97m"}
            };*/

            constexpr std::array<const char*, 18> COLOR_MAP = 
            {
				"\033[0m",        // RESET
				"\033[30m",       // BLACK
				"\033[31m",       // RED
				"\033[32m",       // GREEN
				"\033[33m",       // YELLOW
				"\033[34m",       // BLUE
				"\033[35m",       // MAGENTA
				"\033[36m",       // CYAN
				"\033[0m",        // WHITE (was duplicated)
				"\033[37m",       // GRAY
				"\033[1m\033[90m", // BOLD_BLACK
				"\033[1m\033[91m", // BOLD_RED
				"\033[1m\033[92m", // BOLD_GREEN
				"\033[1m\033[93m", // BOLD_YELLOW
				"\033[1m\033[94m", // BOLD_BLUE
				"\033[1m\033[95m", // BOLD_MAGENTA
				"\033[1m\033[96m", // BOLD_CYAN
				"\033[1m\033[97m"  // BOLD_WHITE
            };

            inline const char* GetColor(LogColor a_color)
            {
                return COLOR_MAP[static_cast<size_t>(a_color)];
            }
	    }
    }
}

#endif