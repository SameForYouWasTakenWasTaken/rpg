#pragma once

#include <Entity.hpp>

#include "EngineContext.hpp"
#include "Types.hpp"

namespace ssg::inventory
{
bool AddItem(EngineContext& engineContext, entt::registry& registry, Entity owner,
             Entity item = entt::null);
void Equip(EngineContext& engineContext, entt::registry& registry, Entity owner, size_t index);
void Unequip(EngineContext& engineContext, entt::registry& registry, Entity owner, size_t slot);
void Drop(EngineContext& engineContext, entt::registry& registry, Entity owner, size_t index,
          ItemCount_t amount = 1);
void RemoveItemFromInventory(EngineContext& engineContext, entt::registry& registry, Entity owner,
                             size_t index, ItemCount_t amount);
void RemoveItemFromHotbar(EngineContext& engineContext, entt::registry& registry, Entity owner,
                          size_t slot);
std::optional<size_t> FindItemIndex(EngineContext& engineContext, entt::registry& registry,
                                    Entity owner, std::string_view itemType);

} // namespace ssg::inventory