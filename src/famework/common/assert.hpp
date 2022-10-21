#pragma once

// Standard Library
#include <cstdio>

// FIXME get this to work
template <typename ...Args>
void _log(const char * prefix, TextColor color, const char * msg, Args ...args) {
    char fmtBuffer[32000] = {};
    char msgBuffer[32000] = {};
    sprintf(fmtBuffer, "%s: %s\n", prefix, msg);
    sprintf(msgBuffer, fmtBuffer, args...);

    platform_log(msgBuffer, color);
}

#define SE_TRACE(msg, ...) _log("TRACE", TEXT_COLOR_GREEN    , msg, __VA_ARGS__)
#define SE_WARN(msg, ...)  _log("WARN",  TEXT_COLOR_YELLOW   , msg, __VA_ARGS__)
#define SE_ERROR(msg, ...) _log("ERROR", TEXT_COLOR_RED      , msg, __VA_ARGS__)
#define SE_FATAL(msg, ...) _log("FATAL", TEXT_COLOR_LIGHT_RED, msg, __VA_ARGS__)

#ifdef DEBUG
#define SE_ASSERT(x, msg, ...)          \
    {                                   \
    if (!(x)){                          \
        SE_ERROR(msg, __VA_ARGS__);     \
        __debugbreak();                 \
    }                                   \
}
#elif
#define SE_ASSERT(x, msg, ...)
#endif