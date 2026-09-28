#pragma once

#include <Entity.hpp>

#include "CItem.hpp"
#include "Types.hpp"

namespace ssg
{
struct InventoryEntry
{
    enum class Kind
    {
        Entity,
        Data
    };
    Kind kind = Kind::Entity;
    Entity entity = entt::null;
    CItem data;
};

struct CInventory
{
    Vector<Entity> items{};
    size_t capacity{0};
};
} // namespace ssg