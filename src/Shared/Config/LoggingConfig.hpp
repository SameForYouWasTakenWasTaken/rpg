#pragma once

namespace ssg::Config::Logging
{
/*
 * Maximum number of entries that can be stored in the memory
 * logging sink at once.
 *
 * Each log message occupies one entry in the sink.
 */
constexpr std::size_t MEM_SINK_MAX_CAPACITY = 1024;
} // namespace ssg::Config::Logging