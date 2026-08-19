#pragma once

#include <entt/entt.hpp>

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
    entt::entity entity = entt::null;
    CItem data;
};

struct CInventory
{
    Vector<entt::entity> items{};
    size_t capacity{0};
};
} // namespace ssg