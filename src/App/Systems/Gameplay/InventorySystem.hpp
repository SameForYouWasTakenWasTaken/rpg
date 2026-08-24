#pragma once

#include <entt/entt.hpp>

#include "Types.hpp"

namespace ssg::inventory
{
bool AddItem(entt::registry& registry, entt::entity owner, entt::entity entity = entt::null);
void RemoveItem(entt::registry& registry, entt::entity owner, size_t index);
void Equip(entt::registry& registry, entt::entity owner, size_t index);
void Unequip(entt::registry& registry, entt::entity owner, size_t slot);
void Drop(entt::registry& registry, entt::entity owner, size_t index, ItemCount_t amount);
std::optional<size_t> FindItemIndex(entt::registry& registry, entt::entity owner,
                                    std::string_view itemType);

} // namespace ssg::inventory