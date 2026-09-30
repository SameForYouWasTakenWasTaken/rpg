#pragma once

#include <Entity.hpp>
#include <cstdint>

#include "Entity.hpp"

namespace ssg::hierarchy
{

// Optional behaviours layered on top of a plain AttachChild. Values are
// distinct bits so this can grow into a bit flag set later if needed.
enum class AttachMode : std::uint8_t
{
    KeepLocal = 0,      // default: child's local transform is left untouched
    KeepWorld = 1 << 0, // rebase local so the child's world position is unchanged
};

// Pure hierarchy attach: links `child` under `parent` in the linked list.
// Knows nothing about transforms.
void AttachChild(entt::registry& registry, Entity parent, Entity child);

// Runs AttachChild, then applies the extra behaviour selected by `mode`.
void AttachChild(entt::registry& registry, Entity parent, Entity child, AttachMode mode);

void DetachChild(entt::registry& registry, Entity child);

bool IsRoot(entt::registry& registry, Entity e);

Entity GetRoot(entt::registry& registry, Entity e);

} // namespace ssg::hierarchy
