#pragma once

// clang-format off
#ifdef USE_LOGGER
  #define LOGGER_LEVEL_DEBUG 1
  #define LOGGER_LEVEL_INFO 2
  #define LOGGER_LEVEL_WARN 3
  #define LOGGER_LEVEL_ERROR 4
  #define LOGGER_LEVEL_CRITICAL 5
  #define LOGGER_LEVEL_OFF 6

  #ifndef LOGGER_LEVEL
    #define LOGGER_LEVEL LOGGER_LEVEL_INFO
  #endif

  #ifdef USE_SPDLOG
    #define SPDLOG_LEVEL_TRACE 0
    #define SPDLOG_LEVEL_DEBUG 1
    #define SPDLOG_LEVEL_INFO 2
    #define SPDLOG_LEVEL_WARN 3
    #define SPDLOG_LEVEL_ERROR 4
    #define SPDLOG_LEVEL_CRITICAL 5
    #define SPDLOG_LEVEL_OFF 6

    #define SPDLOG_ACTIVE_LEVEL LOGGER_LEVEL

    #include "spdlog/spdlog.h"
    #include <memory>

    namespace seg {
    class Logger {
    public:
      static spdlog::logger* getInstance();
    };
    }  // namespace seg

    #define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(seg::Logger::getInstance(), __VA_ARGS__)
    #define LOG_INFO(...)  SPDLOG_LOGGER_INFO(seg::Logger::getInstance(), __VA_ARGS__)
    #define LOG_WARN(...)  SPDLOG_LOGGER_WARN(seg::Logger::getInstance(), __VA_ARGS__)
    #define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(seg::Logger::getInstance(), __VA_ARGS__)
    #define LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(seg::Logger::getInstance(), __VA_ARGS__)
    #define SET_LOG_LEVEL(lvl) seg::Logger::getInstance()->set_level(static_cast<spdlog::level::level_enum>(lvl))

    #define LOGGER_DEFINED
  #endif // USE_SPDLOG

  #ifndef LOGGER_DEFINED
    #error "USE_LOGGER set true but no logger backend is defined."
  #endif

#else // ! USE_LOGGER
  #define LOG_DEBUG(...) (void)0
  #define LOG_INFO(...)  (void)0
  #define LOG_WARN(...)  (void)0
  #define LOG_ERROR(...) (void)0
  #define LOG_FATAL(...) (void)0
  #define SET_LOG_LEVEL(lvl) (void)0

  #define FMT_HEADER_ONLY
  #include "seg/internal/fmt/core.h"

  #define PRINT(...) fmt::print("[SEG] {}\n",fmt::format(__VA_ARGS__))
#endif // USE_LOGGER
// clang-format on
