#include "Logger.hpp"

#include <iostream>
#include <time.h>
#include <utility>

namespace ssg::log
{
void MemorySink::Write(const LogEntry& entry)
{
    entries.push_back(entry);
    if (entries.size() > Capacity)
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
    const auto time = std::chrono::system_clock::to_time_t(entry.time);
    std::tm localTime{};

    localtime_s(&localTime, &time);

    std::cout << std::format("[{:02}:{:02}:{:02}] [{}] [{}] [{}] : {}\n", localTime.tm_hour,
                             localTime.tm_min, localTime.tm_sec, tag, entry.category, entry.source,
                             entry.message);
}
void Logger::AddSink(std::unique_ptr<ILogSink> sink) { m_Sinks.push_back(std::move(sink)); }
void Logger::Dispatch(const LogEntry& entry)
{
    for (auto& sink : m_Sinks)
        if (entry.level >= sink->minLevel)
            sink->Write(entry);
}
} // namespace ssg::log
