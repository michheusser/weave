// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MACROS_H_2025_09_02_12_41_16
#define MACROS_H_2025_09_02_12_41_16

#include <weave/logging/Definitions.h>
#include <weave/logging/Logger.h>

#if WEAVE_ENABLE_LOGGING
    // Do-while(0) pattern to ensure scoping and correct semicolon enforcement after macro
    #define LOG_LOGGER_CALL(level, ...) \
        do { \
            auto& logger = weave::logging::Logger::getInternalLogger(); \
            if (logger && logger->should_log(level)) { \
                logger->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, level, __VA_ARGS__); \
            } \
        } while(0)

    #if WEAVE_ENABLE_TRACE
        #define LOG_TRACE(...) LOG_LOGGER_CALL(spdlog::level::trace, __VA_ARGS__)
    #else
        #define LOG_TRACE(...) ((void)0)
    #endif

    #if WEAVE_ENABLE_DEBUG
        #define LOG_DEBUG(...) LOG_LOGGER_CALL(spdlog::level::debug, __VA_ARGS__)
    #else
        #define LOG_DEBUG(...) ((void)0)
    #endif

    #if WEAVE_ENABLE_INFO
        #define LOG_INFO(...) LOG_LOGGER_CALL(spdlog::level::info, __VA_ARGS__)
    #else
        #define LOG_INFO(...) ((void)0)
    #endif

    #if WEAVE_ENABLE_WARN
        #define LOG_WARN(...) LOG_LOGGER_CALL(spdlog::level::warn, __VA_ARGS__)
    #else
        #define LOG_WARN(...) ((void)0)
    #endif

    #if WEAVE_ENABLE_ERROR
        #define LOG_ERROR(...) LOG_LOGGER_CALL(spdlog::level::err, __VA_ARGS__)
    #else
        #define LOG_ERROR(...) ((void)0)
    #endif

    #if WEAVE_ENABLE_FATAL
        #define LOG_FATAL(...) LOG_LOGGER_CALL(spdlog::level::critical, __VA_ARGS__)
    #else
        #define LOG_FATAL(...) ((void)0)
    #endif

#else
    // Complete elimination in release builds
    #define LOG_TRACE(...) ((void)0)
    #define LOG_DEBUG(...) ((void)0)
    #define LOG_INFO(...)  ((void)0)
    #define LOG_WARN(...)  ((void)0)
    #define LOG_ERROR(...) ((void)0)
    #define LOG_FATAL(...) ((void)0)
#endif

#endif