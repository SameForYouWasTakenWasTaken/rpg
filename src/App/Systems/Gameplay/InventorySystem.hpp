#pragma once

#include <entt/entt.hpp>

#include "Types.hpp"

namespace ssg::inventory
{
bool AddItem(entt::registry& registry, entt::entity owner, entt::entity item = entt::null);
void Equip(entt::registry& registry, entt::entity owner, size_t index);
void Unequip(entt::registry& registry, entt::entity owner, size_t slot);
void Drop(entt::registry& registry, entt::entity owner, size_t index, ItemCount_t amount = 1);
void RemoveItemFromInventory(entt::registry& registry, entt::entity owner, size_t index,
                             ItemCount_t amount);
void RemoveItemFromHotbar(entt::registry& registry, entt::entity owner, size_t slot);
std::optional<size_t> FindItemIndex(entt::registry& registry, entt::entity owner,
                                    std::string_view itemType);

} // namespace ssg::inventory