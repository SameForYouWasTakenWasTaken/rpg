#include "Logger.hpp"

#include <chrono>
#include <iostream>
#include <time.h>
#include <utility>

#include "Config/Config.hpp"

namespace ssg::log
{
void MemorySink::Write(const LogEntry& entry)
{
    entries.push_back(entry);
    if (entries.size() > Config::Logging::MEM_SINK_MAX_CAPACITY)
        entries.pop_front();
}

void ConsoleSink::Write(const LogEntry& entry)
{
    std::string tag = {};
    switch (entry.level)
    {
    case LogLevel::Trace:
        tag = "TRACE";
        break;
    case LogLevel::Debug:
        tag = "DEBUG";
        break;
    case LogLevel::Info:
        tag = "INFO";
        break;
    case LogLevel::Warn:
        tag = "WARN";
        break;
    case LogLevel::Error:
        tag = "ERROR";
        break;
    case LogLevel::Fatal:
        tag = "FATAL";
        break;
    }

    // remove floating point
    auto formattedTime = std::chrono::floor<std::chrono::seconds>(entry.time);

    std::cout << std::format("[{:%H:%M:%S}] [{}] [{}] [{}] : {}\n", formattedTime, tag,
                             entry.category, entry.source, entry.message);
}
void Logger::AddSink(std::unique_ptr<ILogSink> sink) { m_Sinks.push_back(std::move(sink)); }
void Logger::Dispatch(const LogEntry& entry)
{
    for (auto& sink : m_Sinks)
        if (entry.level >= m_MinLevel && entry.level >= sink->minLevel)
            sink->Write(entry);
}
} // namespace ssg::log
