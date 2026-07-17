#pragma once

#include "Types.hpp"

namespace ssg
{

struct CTransform
{
    Vec2 position{0.0f, 0.0f};
    Vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};
};

} // namespace ssg