#pragma once

#include "Types.hpp"

namespace ssg::Config::Rendering
{
/*
 * The renderer orders sprite batches by their z-index.
 *
 * Therefore, the number of distinct rendering layers is limited by the
 * range of zIndex_t. For example, if zIndex_t is uint8_t, there are
 * 256 possible z-index values (0-255), giving 256 distinct ordering
 * positions.
 */
constexpr std::size_t Z_INDEX_LAYER_COUNT =
    static_cast<std::size_t>(std::numeric_limits<zIndex_t>::max()) + 1;
} // namespace ssg::Config::Rendering