#pragma once

#include <string>

#include "Types.hpp"

namespace ssg
{
struct CWeapon
{
    std::string name = "";
    float damage = 0.0f;
    float range = 0.0f;

    float attackSpeed = 1.0f;    // attacks per second, MUST be > 0
    Vec2 hitWindow = {0.f, 0.f}; // x = min, y = max, both values are *NORMALIZED*
};
} // namespace ssg