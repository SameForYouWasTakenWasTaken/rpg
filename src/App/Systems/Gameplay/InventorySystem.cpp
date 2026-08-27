#include "InventorySystem.hpp"

#include <cassert>
#include <cstddef>
#include <optional>

#include "Components/CDefinition.hpp"
#include "Components/CSprite.hpp"
#include "Components/CTransform.hpp"
#include "Components/Gameplay/CEquipment.hpp"
#include "Components/Gameplay/CWeapon.hpp"
#include "Components/Gameplay/Inventory/CInventory.hpp"
#include "Components/Gameplay/Inventory/CItem.hpp"
#include "Config/Gameplay/InventoryConfig.hpp"
#include "Factories/Default.hpp"
#include "Factories/Gameplay/Items.hpp"
#include "Logging.hpp"
#include "Systems/Hierarchy.hpp"

namespace ssg::inventory
{

namespace
{

bool IsValidIndex(const CInventory& inventory, size_t index)
{
    return index < inventory.items.size();
}

} // namespace

bool AddItem(entt::registry& registry, entt::entity owner, entt::entity item)
{
    auto& inventory = registry.get<CInventory>(owner);
    auto& incoming = registry.get<CItem>(item);

    // Nothing left to add
    if (incoming.currentCount == 0)
        return true;

    // Need a new inventory slot for the remaining stack
    if (inventory.items.size() >= inventory.capacity)
        return false;

    // First fill existing stacks
    for (auto entity : inventory.items)
    {
        auto& current = registry.get<CItem>(entity);

        if (current.itemTypeId != incoming.itemTypeId)
            continue;

        const ItemCount_t space = current.maxStack - current.currentCount;
        const ItemCount_t amountToAdd = std::min(space, incoming.currentCount);

        current.currentCount += amountToAdd;
        incoming.currentCount -= amountToAdd;

        if (incoming.currentCount == 0)
        {
            registry.destroy(item);
            return true;
        }
    }

    inventory.items.push_back(item);
    return true;
}

std::optional<size_t> FindItemIndex(entt::registry& registry, entt::entity owner,
                                    std::string_view itemType)
{
    auto& inventory = registry.get<CInventory>(owner);

    size_t index = 0;

    for (auto entity : inventory.items)
    {
        auto& current = registry.get<CItem>(entity);

        if (current.itemTypeId == itemType)
            return index;

        ++index;
    }

    return std::nullopt;
}

void Equip(entt::registry& registry, entt::entity owner, size_t index)
{
    auto& inventory = registry.get<CInventory>(owner);
    auto& equipment = registry.get<CEquipment>(owner);
    if (index >= inventory.items.size())
    {
        LOG_ERROR("Inventory", "Inventory index {} is out of bounds! Size: {}", index,
                  inventory.items.size());
        return;
    }

    entt::entity item = inventory.items[index];

    if (item == entt::null)
    {
        LOG_ERROR("Inventory", "Item to equip at index {} is null!", index);
        return;
    }

    auto& itemData = registry.get<CItem>(item);

    // TODO: Create an CEquippable or something similar, as not to use CWeapon
    if (!registry.all_of<CWeapon>(item))
        return;

    // Put the old weapon back into the inventory.
    if (equipment.weapon != entt::null)
    {
        inventory.items.push_back(equipment.weapon);
    }

    equipment.weapon = item;

    const auto offset = static_cast<std::ptrdiff_t>(index);
    inventory.items.erase(inventory.items.begin() + offset);

    LOG_INFO("Inventory", "Equipped {}", itemData.itemTypeId);
}

void Unequip(entt::registry& registry, entt::entity owner, size_t slot)
{
    // slot is currently unused.
    (void)slot;

    auto& equipment = registry.get<CEquipment>(owner);

    if (equipment.weapon == entt::null)
        return;

    entt::entity weapon = equipment.weapon;

    auto& itemData = registry.get<CItem>(weapon);

    if (AddItem(registry, owner, weapon))
    {
        LOG_INFO("Inventory", "Unequipped {}", itemData.itemTypeId);

        equipment.weapon = entt::null;
        return;
    }

    LOG_WARN("Inventory", "Couldn't unequip {}", itemData.itemTypeId);
}

void Drop(entt::registry& registry, entt::entity owner, size_t index, ItemCount_t amount)
{
    auto& inventory = registry.get<CInventory>(owner);
    if (index >= inventory.items.size())
    {
        LOG_ERROR("Inventory", "Inventory index {} is out of bounds! Size: {}", index,
                  inventory.items.size());
        return;
    }

    entt::entity inventoryItem = inventory.items[index];

    if (inventoryItem == entt::null)
    {
        LOG_ERROR("Inventory", "Inventory item at index {} is null!", index);
        return;
    }

    auto& item = registry.get<CItem>(inventoryItem);

    assert(amount > 0 && "Drop amount must be greater than zero!");

    // ReSharper disable once CppDFAConstantConditions
    if (amount == 0)
    {
        LOG_ERROR("Inventory", "Cannot drop item {} zero items.", item.itemTypeId);
        return;
    }

    if (amount > item.currentCount)
    {
        LOG_ERROR("Inventory", "Drop count {} of item {} exceeds item count {}!", amount,
                  item.itemTypeId, item.currentCount);
        return;
    }

    auto& ownerTransform = registry.get<CTransform>(owner);
    auto& ownerSprite = registry.get<CSprite>(owner);

    Vec2 facingDirection{};
    switch (ownerSprite.facingDirection)
    {
    case Direction::UP:
        facingDirection = {0.f, 1.f};
        break;

    case Direction::DOWN:
        facingDirection = {0.f, -1.f};
        break;

    case Direction::RIGHT:
        facingDirection = {1.f, 0.f};
        break;

    case Direction::LEFT:
        facingDirection = {-1.f, 0.f};
        break;
    }

    // Create a new entity representing the stack that was dropped into the world.
    auto& itemDefinition = registry.get<CDefinition>(inventoryItem);
    entt::entity worldItem = registry.create();
    factory::ApplyItemDefinition(registry, worldItem, itemDefinition.filepath);

    auto& worldItemData = registry.get<CItem>(worldItem);
    worldItemData.currentCount = amount; // current count is the items dropped

    auto& itemTransform = registry.get<CTransform>(worldItem);
    itemTransform.position = ownerTransform.position + ownerSprite.origin * ownerSprite.size;
    itemTransform.position +=
        facingDirection * (ownerSprite.size + Config::Inventory::DEFAULT_ITEM_DROP_DISTANCE);

    RemoveItemFromInventory(registry, owner, index, amount);

    LOG_INFO("Inventory", "Dropped {}x {}", amount, worldItemData.itemTypeId);
}

void RemoveItemFromInventory(entt::registry& registry, entt::entity owner, size_t index,
                             ItemCount_t amount)
{
    auto& inventory = registry.get<CInventory>(owner);

    if (index >= inventory.items.size())
    {
        LOG_ERROR("Inventory", "Inventory index {} is out of bounds! Size: {}", index,
                  inventory.items.size());

        return;
    }

    entt::entity itemEntity = inventory.items[index];

    if (itemEntity == entt::null)
    {
        LOG_ERROR("Inventory", "Inventory item at index {} is null!", index);

        return;
    }

    auto& item = registry.get<CItem>(itemEntity);
    if (amount > item.currentCount)
    {
        LOG_ERROR("Inventory", "Remove count {} exceeds item count {}!", amount, item.currentCount);

        return;
    }

    item.currentCount -= amount;

    // delete from registry if count is 0
    if (item.currentCount <= 0)
    {
        const auto offset = static_cast<std::ptrdiff_t>(index);

        inventory.items.erase(inventory.items.begin() + offset);
        hierarchy::DetachChild(registry, itemEntity); // Discard any relationships
        registry.destroy(itemEntity);
    }
}

void RemoveItemFromHotbar(entt::registry& registry, entt::entity owner, size_t slot)
{
    // TODO
}

} // namespace ssg::inventory