#pragma once

#include "Engine.hpp" // dont remove

#define LOG_TRACE(category, ...)                                                                   \
    Engine::instance().logger.Trace(std::source_location::current(), category, __VA_ARGS__)

#define LOG_DEBUG(category, ...)                                                                   \
    Engine::instance().logger.Debug(std::source_location::current(), category, __VA_ARGS__)

#define LOG_INFO(category, ...)                                                                    \
    Engine::instance().logger.Info(std::source_location::current(), category, __VA_ARGS__)

#define LOG_WARN(category, ...)                                                                    \
    Engine::instance().logger.Warn(std::source_location::current(), category, __VA_ARGS__)

#define LOG_ERROR(category, ...)                                                                   \
    Engine::instance().logger.Error(std::source_location::current(), category, __VA_ARGS__)

#define LOG_FATAL(category, ...)                                                                   \
    Engine::instance().logger.Fatal(std::source_location::current(), category, __VA_ARGS__)
