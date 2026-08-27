#pragma once
#include "Types.hpp"

namespace ssg::Config::Inventory
{
constexpr InventorySlot_t MAX_INVENTORY_SLOTS = 32;       // Not implemented
constexpr ItemCount_t MAX_INVENTORY_STACK = 100;          // Not implemented
constexpr Vec2 DEFAULT_ITEM_DROP_DISTANCE = {100.f, 0.f}; // In pixels
} // namespace ssg::Config::Inventory