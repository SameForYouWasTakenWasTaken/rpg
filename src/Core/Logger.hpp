#pragma once

#include <chrono>
#include <concepts>
#include <cstdint>
#include <deque>
#include <format>
#include <memory>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

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

// A file + line pair. Built from a std::source_location for C++ callers,
// or from a plain file/line for locations that don't come from C++ (Lua scripts).
struct LogLocation
{
    std::string file;
    uint32_t line;

    LogLocation(const std::source_location& l) : file(l.file_name()), line(l.line()) {}

    LogLocation(std::string f, uint32_t ln) : file(std::move(f)), line(ln) {}
};

// Bundles the format string with the caller's source location.
// std::source_location::current() as a default argument is evaluated at the call site, but a
// defaulted parameter can't follow a parameter pack. Putting it in the constructor of the
// format-string parameter gets around that, so Info("cat", "msg {}", x) records where it was
// called from instead of a line inside this header.
template <typename... Args> struct FormatWithLocation
{
    std::format_string<Args...> fmt;
    std::source_location loc;

    template <typename T>
        requires std::convertible_to<const T&, std::string_view>
    consteval FormatWithLocation(const T& s,
                                 std::source_location l = std::source_location::current())
        : fmt(s), loc(l)
    {
    }
};

struct ILogSink
{
    virtual ~ILogSink() = default;
    virtual void Write(const LogEntry& entry) = 0;
    LogLevel minLevel = LogLevel::Trace;
};

template <typename T>
concept LogSink = std::derived_from<T, ILogSink>;

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

    template <LogSink T> T* FindSink()
    {
        for (auto& sink : m_Sinks)
        {
            if (auto* result = dynamic_cast<T*>(sink.get()))
                return result;
        }

        return nullptr;
    }

    template <LogSink T> T& GetSink()
    {
        if (auto* sink = FindSink<T>())
            return *sink;

        throw std::runtime_error("Requested log sink is not registered!");
    }

    void SetLevel(LogLevel level) { m_MinLevel = level; }

    // Full logging function (explicit location, e.g. from Lua)
    template <typename... Args>
    void Log(LogLevel level, LogLocation loc, std::string_view category,
             std::format_string<Args...> fmt, Args&&... args) const;

    // Location taken automatically from the call site
    template <typename... Args>
    void Log(LogLevel level, std::string_view category,
             FormatWithLocation<std::type_identity_t<Args>...> fmt, Args&&... args) const;

    // Convenience functions: explicit location
    template <typename... Args>
    void Info(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
              Args&&... args) const;
    template <typename... Args>
    void Warn(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
              Args&&... args) const;
    template <typename... Args>
    void Error(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args) const;
    template <typename... Args>
    void Fatal(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args) const;
    template <typename... Args>
    void Trace(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args) const;
    template <typename... Args>
    void Debug(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
               Args&&... args) const;

    // Convenience functions: location taken automatically from the call site
    template <typename... Args>
    void Info(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
              Args&&... args) const;
    template <typename... Args>
    void Warn(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
              Args&&... args) const;
    template <typename... Args>
    void Error(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
               Args&&... args) const;
    template <typename... Args>
    void Fatal(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
               Args&&... args) const;
    template <typename... Args>
    void Trace(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
               Args&&... args) const;
    template <typename... Args>
    void Debug(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
               Args&&... args) const;

  private:
    void Dispatch(const LogEntry& entry) const;

    Vector<std::unique_ptr<ILogSink>> m_Sinks;
    LogLevel m_MinLevel = LogLevel::Info;
};

// ============================================================
// Log
// ============================================================

template <typename... Args>
void Logger::Log(LogLevel level, LogLocation loc, std::string_view category,
                 std::format_string<Args...> fmt, Args&&... args) const
{
    const String message = std::format(fmt, std::forward<Args>(args)...);
    const String categoryName = std::string(category);
    const String source = loc.file + ":" + std::to_string(loc.line);
    const auto time = std::chrono::system_clock::now();

    LogEntry entry{level, message, categoryName, source, time};
    Dispatch(entry);
}

template <typename... Args>
void Logger::Log(LogLevel level, std::string_view category,
                 FormatWithLocation<std::type_identity_t<Args>...> fmt, Args&&... args) const
{
    Log(level, LogLocation(fmt.loc), category, fmt.fmt, std::forward<Args>(args)...);
}

// ============================================================
// Info
// ============================================================

template <typename... Args>
void Logger::Info(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
                  Args&&... args) const
{
    Log(LogLevel::Info, std::move(loc), category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Info(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
                  Args&&... args) const
{
    Log(LogLevel::Info, LogLocation(fmt.loc), category, fmt.fmt, std::forward<Args>(args)...);
}

// ============================================================
// Warn
// ============================================================

template <typename... Args>
void Logger::Warn(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
                  Args&&... args) const
{
    Log(LogLevel::Warn, std::move(loc), category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Warn(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
                  Args&&... args) const
{
    Log(LogLevel::Warn, LogLocation(fmt.loc), category, fmt.fmt, std::forward<Args>(args)...);
}

// ============================================================
// Error
// ============================================================

template <typename... Args>
void Logger::Error(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Error, std::move(loc), category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Error(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Error, LogLocation(fmt.loc), category, fmt.fmt, std::forward<Args>(args)...);
}

// ============================================================
// Fatal
// ============================================================

template <typename... Args>
void Logger::Fatal(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Fatal, std::move(loc), category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Fatal(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Fatal, LogLocation(fmt.loc), category, fmt.fmt, std::forward<Args>(args)...);
}

// ============================================================
// Trace
// ============================================================

template <typename... Args>
void Logger::Trace(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Trace, std::move(loc), category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Trace(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Trace, LogLocation(fmt.loc), category, fmt.fmt, std::forward<Args>(args)...);
}

// ============================================================
// Debug
// ============================================================

template <typename... Args>
void Logger::Debug(LogLocation loc, std::string_view category, std::format_string<Args...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Debug, std::move(loc), category, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::Debug(std::string_view category, FormatWithLocation<std::type_identity_t<Args>...> fmt,
                   Args&&... args) const
{
    Log(LogLevel::Debug, LogLocation(fmt.loc), category, fmt.fmt, std::forward<Args>(args)...);
}

} // namespace ssg::log