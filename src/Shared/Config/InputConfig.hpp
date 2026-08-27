#pragma once

namespace ssg::Config::Input
{
/*
 * Time a key must remain held before automatic key-repeat events begin.
 */
constexpr float REPEAT_DELAY = 1.0f;

/*
 * Time between consecutive automatic key-repeat events while a key
 * remains held.
 */
constexpr float REPEAT_INTERVAL = 0.1f;
} // namespace ssg::Config::Input