//----------------------------------------------------------------------------------------
/**
 * \file       logging.hpp
 * \author     Yury Udavichenka
 * \date       21/10/2022
 * \brief      Logger header file
 *
 *  Simple multi-platform header file used for logging and assertion.
 *  TODO add support for more compilers/platforms, currently only Windows/MSVC support is properly implemented
 */
//----------------------------------------------------------------------------------------
#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <cstdio>
#include <cstring>
#include "types.hpp"

// FIXME get this to work
namespace se_internals {
    enum Color {
        TEXT_COLOR_RED,
        TEXT_COLOR_YELLOW,
        TEXT_COLOR_GREEN,
        TEXT_COLOR_LIGHT_RED,
        TEXT_COLOR_WHITE
    };

#ifdef _WIN32
    static u16 get_log_color_bits(se_internals::Color color) {
        u16 colorBits = 0;

        switch (color) {
            case TEXT_COLOR_RED:
                colorBits = FOREGROUND_RED | FOREGROUND_INTENSITY;
                break;
            case TEXT_COLOR_YELLOW:
                colorBits = FOREGROUND_GREEN | FOREGROUND_RED;
                break;
            case TEXT_COLOR_GREEN:
                colorBits = FOREGROUND_GREEN;
                break;
            case TEXT_COLOR_LIGHT_RED:
                colorBits = FOREGROUND_RED;
                break;
            case TEXT_COLOR_WHITE:
                colorBits = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                break;
            default:    // means we should probably define more colours! Hence the annoying combination
                colorBits = FOREGROUND_BLUE | BACKGROUND_RED;
                break;
        }

        return colorBits;
    }

    static void windows_log(const char * msg, const se_internals::Color color) {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        u16 colorBits = get_log_color_bits(color);
        SetConsoleTextAttribute(consoleHandle, colorBits);

    #ifdef DEBUG
        OutputDebugStringA(msg);
    #endif
        WriteConsoleA(consoleHandle, msg, (u32) strlen(msg), nullptr, nullptr);
        // Reset log color
        colorBits = get_log_color_bits(se_internals::TEXT_COLOR_WHITE);
        SetConsoleTextAttribute(consoleHandle, colorBits);
    }
#endif

#ifdef  __linux__ 
    static const char * get_log_color_symbols(se_internals::Color color) {
        switch (color) {
            case TEXT_COLOR_RED:
                return "\x1B[31m";
            case TEXT_COLOR_YELLOW:
                return "\x1B[33m";
            case TEXT_COLOR_GREEN:
                return "\x1B[32m";
            case TEXT_COLOR_LIGHT_RED:
                return "\x1B[35m";
            case TEXT_COLOR_WHITE:
                return "\x1B[37m";
            default:    // means we should probably define more colours! Hence the annoying combination
                return "\x1B[37m";
        }
    }
#endif

#ifdef _WIN32
#define SE_SPRINTF sprintf_s
#else
#define SE_SPRINTF sprintf
#endif

    static void platform_log(const char * msg, const se_internals::Color color) {
#ifdef _WIN32
        windows_log(msg, color);
#elif __linux__
        // TODO add colours
        const char * prefix = get_log_color_symbols(color);
        // to reset terminal colour to default
        const char * postfix = "\x1B[0m";
        printf("%s%s%s", prefix, msg, postfix);
#else
        // TODO can not log, platform not defined
#endif
    }

    template<typename ...Args>
    static void se_log(const char * prefix, const se_internals::Color color, const char * msg, Args ...args) {
        char fmtBuffer[32000] = {};
        char msgBuffer[32000] = {};
        SE_SPRINTF(fmtBuffer, "%s: %s\n", prefix, msg);

// trust me, I'm an engineer
#ifdef __linux__
#pragma GCC diagnostic error "-Wformat-security"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
        SE_SPRINTF(msgBuffer, fmtBuffer, args...);
#ifdef __linux__
#pragma GCC diagnostic pop
#endif
        platform_log(msgBuffer, color);
    }

    static void line_and_file_info(const char * fileName, int lineNumber) {
        char buffer[32000] = {};
        SE_SPRINTF(buffer, "%s:%d: ", fileName, lineNumber);

        platform_log(buffer, TEXT_COLOR_WHITE);
    }
} /* namespace se_internals */

#define SE_LINEINFO
#define SE_TRACE(msg, ...) se_internals::se_log("TRACE", se_internals::TEXT_COLOR_GREEN,     msg __VA_OPT__(,) __VA_ARGS__)
#define SE_WARN(msg, ...)  se_internals::se_log("WARN",  se_internals::TEXT_COLOR_YELLOW,    msg __VA_OPT__(,) __VA_ARGS__)
#define SE_ERROR(msg, ...) se_internals::se_log("ERROR", se_internals::TEXT_COLOR_RED,       msg __VA_OPT__(,) __VA_ARGS__)
#define SE_FATAL(msg, ...) do {                                                                                      \
        se_internals::se_log("FATAL", se_internals::TEXT_COLOR_LIGHT_RED, msg __VA_OPT__(,) __VA_ARGS__ );           \
        SE_DEBUG_BREAK;                                                                                              \
    } while (0)

#ifdef _MSC_VER // MSVC
#define SE_DEBUG_BREAK __debugbreak()
#elif __clang__
#define SE_DEBUG_BREAK __builtin_debugtrap()
#else   // TODO expand
#define SE_DEBUG_BREAK
#endif

// TODO extract this
#define DEBUG

#if defined DEBUG || defined _DEBUG
#define SE_ASSERT(x, msg, ...)                                  \
    {                                                           \
    if (!(x)){                                                  \
        se_internals::line_and_file_info(__FILE__, __LINE__);   \
        SE_ERROR(msg, __VA_ARGS__);                             \
        SE_DEBUG_BREAK;                                         \
    }                                                           \
}
#else
#define SE_ASSERT(x, msg, ...)
#endif