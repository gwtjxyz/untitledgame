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
    static u32 get_log_color_bits(se_internals::Color color) {
        u32 colorBits = 0;

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
        u32 colorBits = get_log_color_bits(color);
        SetConsoleTextAttribute(consoleHandle, colorBits);

    #ifdef DEBUG
            OutputDebugStringA(msg);
    #endif
        WriteConsoleA(consoleHandle, msg, strlen(msg), 0, 0);
    }
#endif

    static void platform_log(const char * msg, const se_internals::Color color) {
#ifdef _WIN32
        windows_log(msg, color);
#elif __linux__
        // TODO Linux logger
#else
        // TODO can not log, platform not defined
#endif
    }

    template<typename ...Args>
    void log(const char * prefix, const se_internals::Color color, const char * msg, Args ...args) {
        char fmtBuffer[32000] = {};
        char msgBuffer[32000] = {};
        sprintf(fmtBuffer, "%s: %s\n", prefix, msg);
        sprintf(msgBuffer, fmtBuffer, args...);

        platform_log(msgBuffer, color);
    }
} /* namespace se_internals */

#define SE_TRACE(msg, ...) se_internals::log("TRACE", se_internals::TEXT_COLOR_GREEN,     msg, __VA_ARGS__)
#define SE_WARN(msg, ...)  se_internals::log("WARN",  se_internals::TEXT_COLOR_YELLOW,    msg, __VA_ARGS__)
#define SE_ERROR(msg, ...) se_internals::log("ERROR", se_internals::TEXT_COLOR_RED,       msg, __VA_ARGS__)
#define SE_FATAL(msg, ...) se_internals::log("FATAL", se_internals::TEXT_COLOR_LIGHT_RED, msg, __VA_ARGS__)

#ifdef DEBUG
#define SE_ASSERT(x, msg, ...)          \
    {                                   \
    if (!(x)){                          \
        SE_ERROR(msg, __VA_ARGS__);     \
        __debugbreak();                 \
    }                                   \
}
#else
#define SE_ASSERT(x, msg, ...)
#endif