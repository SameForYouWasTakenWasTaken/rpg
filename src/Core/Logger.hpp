#pragma once

#include <chrono>
#include <deque>
#include <format>
#include <memory>
#include <source_location>

#include "Types.hpp"

namespace ssg::log
{
enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

struct LogEntry
{
    LogLevel level;
    String message;
    String category;
    String source;
    std::chrono::system_clock::time_point time;
};

struct ILogSink
{
    virtual ~ILogSink() = default;
    virtual void Write(const LogEntry& entry) = 0;
    LogLevel minLevel = LogLevel::Trace;
};

struct MemorySink : ILogSink
{
    std::deque<LogEntry> entries;

    void Write(const LogEntry& entry) override;
};

// used for std::cout
struct ConsoleSink : ILogSink
{
    void Write(const LogEntry& entry) override;
};

class Logger final
{
  public:
    void AddSink(std::unique_ptr<ILogSink> sink);
    void SetLevel(LogLevel level) { m_MinLevel = level; }

    template <typename... Args>
    void Log(LogLevel level, std::source_location loc, std::string_view category,
             std::format_string<Args...> fmt, Args&&... args);

    MemorySink* GetMemorySink() { return m_MemorySink; } // Reserved for later, currently nullptr

    // Convenience functions
    template <typename... Args>
    void Info(std::source_location loc, std::string_view category, std::format_string<Args...> fmt,
              Args&&... args);

    template <typename... Args>
    void Warn(std::source_location loc, std::string_view category, std::format_string<Args...> fmt,
              Args&&... args);

    template <typename... Args>
    void Error(std::source_location loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args);

    template <typename... Args>
    void Fatal(std::source_location loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args);

    template <typename... Args>
    void Trace(std::source_location loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args);

    template <typename... Args>
    void Debug(std::source_location loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args);

  private:
    void Dispatch(const LogEntry& entry);
    Vector<std::unique_ptr<ILogSink>> m_Sinks;
    MemorySink* m_MemorySink = nullptr; // reserved for later
    LogLevel m_MinLevel = LogLevel::Info;
};

// Logging implementations
template <typename... Args>
void Logger::Log(LogLevel level, std::source_location loc, std::string_view category,
                 std::format_string<Args...> fmt, Args&&... args)
{
    const String message = std::format(fmt, std::forward<Args>(args)...);
    const String categoryName = std::string(category);
    // file:line
    const String source = std::string(loc.file_name()) + ":" + std::to_string(loc.line());
    const auto time = std::chrono::system_clock::now();

    LogEntry entry{level, message, categoryName, source, time};
    Dispatch(entry);
}

template <typename... Args>
void Logger::Info(std::source_location loc, std::string_view category,
                  std::format_string<Args...> fmt, Args&&... args)
{
    Log(LogLevel::Info, loc, category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Warn(std::source_location loc, std::string_view category,
                  std::format_string<Args...> fmt, Args&&... args)
{
    Log(LogLevel::Warn, loc, category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Error(std::source_location loc, std::string_view category,
                   std::format_string<Args...> fmt, Args&&... args)
{
    Log(LogLevel::Error, loc, category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Fatal(std::source_location loc, std::string_view category,
                   std::format_string<Args...> fmt, Args&&... args)
{
    Log(LogLevel::Fatal, loc, category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Trace(std::source_location loc, std::string_view category,
                   std::format_string<Args...> fmt, Args&&... args)
{
    Log(LogLevel::Trace, loc, category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Debug(std::source_location loc, std::string_view category,
                   std::format_string<Args...> fmt, Args&&... args)
{
    Log(LogLevel::Debug, loc, category, fmt, std::forward<Args>(args)...);
}

} // namespace ssg::log