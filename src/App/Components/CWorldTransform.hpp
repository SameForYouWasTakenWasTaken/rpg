#pragma once

#include "Types.hpp"

namespace ssg
{

// Derived, read-only world-space transform.
//
// Filled once per frame by the transform system from CTransform (local) and
// the parent chain in CRelationship. Systems that need absolute position
// (rendering, spatial grid, camera) should read this, never write it.
struct CWorldTransform
{
    Vec2 position{0.0f, 0.0f};
    Vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};
};

} // namespace ssg
