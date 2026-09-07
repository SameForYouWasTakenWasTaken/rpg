#pragma once
#include "Types.hpp"

namespace ssg::map
{
struct MapObject
{
    String name;
    String type;

    float x = 0.f;
    float y = 0.f;

    float width = 0.f;
    float height = 0.f;
};
} // namespace ssg::map