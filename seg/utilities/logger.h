#ifndef SEG_UTILITES_LOGGER_H
#define SEG_UTILITES_LOGGER_H

#ifdef SPDLOG

    #include "spdlog/spdlog.h"

    #define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
    #define LOG_INFO(...) spdlog::info(__VA_ARGS__)
    #define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
    #define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
    #define LOG_FATAL(...) spdlog::critical(__VA_ARGS__)

    #define SET_LOGGER_LEVEL(LEVEL) spdlog::set_level(static_cast<spdlog::level::level_enum>(LEVEL))

#else
    #include <stdio.h>

    #define LOG_DEBUG(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...) printf(__VA_ARGS__)
    #define LOG_FATAL(...) printf(__VA_ARGS__)
    #define SET_LOGGER_LEVEL(LEVEL)

#endif

#endif