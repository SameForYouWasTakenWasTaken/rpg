#include "InventorySystem.hpp"

#include <optional>

#include "Components/Gameplay/CEquipment.hpp"
#include "Components/Gameplay/CWeapon.hpp"
#include "Components/Gameplay/Inventory/CInventory.hpp"
#include "Components/Gameplay/Inventory/CItem.hpp"
#include "InventorySystem.hpp"
#include "Logging.hpp"

namespace ssg::inventory
{
bool AddItem(entt::registry& registry, entt::entity owner, entt::entity item)
{
    auto& inventory = registry.get<CInventory>(owner);
    if (inventory.items.size() >= inventory.capacity)
        return false;

    auto& incoming = registry.get<CItem>(item);
    for (auto& entity : inventory.items)
    {
        auto current = registry.get<CItem>(entity);

        if (current.itemTypeId == incoming.itemTypeId)
        {
            if (current.currentCount + incoming.currentCount <= current.maxStack)
            { // guard too big stack
                current.currentCount += incoming.currentCount;
                registry.destroy(item);
                return true;
            }
        }
    }

    inventory.items.push_back(item); // Entity not found in inventory, append
    return true;
}

std::optional<size_t> FindItemIndex(entt::registry& registry, entt::entity owner,
                                    std::string_view itemType)
{
    auto& inventory = registry.get<CInventory>(owner);

    size_t index = 0;
    for (auto entity : inventory.items)
    {
        auto current = registry.get<CItem>(entity);
        if (current.itemTypeId == itemType)
        {
            return index;
        }
        index++;
    }

    return std::nullopt;
};
void RemoveItem(entt::registry& registry, entt::entity owner, size_t index) {}

void Equip(entt::registry& registry, entt::entity owner, size_t index)
{
    auto& inventory = registry.get<CInventory>(owner);
    auto& equipment = registry.get<CEquipment>(owner);
    assert(index <= inventory.capacity); // OOB

    entt::entity item = inventory.items[index];
    assert(item != entt::null); // OOB

    auto& itemData = registry.get<CItem>(item);

    if (!registry.all_of<CWeapon>(item))
        return; // only equip weapons
    if (equipment.weapon != entt::null)
        inventory.items.push_back(equipment.weapon);        // already has a weapon, swap old
    equipment.weapon = item;                                // set new
    inventory.items.erase(inventory.items.begin() + index); // remove new from inventory
    LOG_INFO("Inventory", "Equipped {}", itemData.itemTypeId);
}

// index is unused, but it is here for more hotbar slots if i were to add them
void Unequip(entt::registry& registry, entt::entity owner, size_t slot)
{
    auto& inventory = registry.get<CInventory>(owner);
    auto& equipment = registry.get<CEquipment>(owner);

    auto& itemData = registry.get<CItem>(equipment.weapon);
    LOG_INFO("Inventory", "Unequipped {}", itemData.itemTypeId);

    inventory.items.push_back(equipment.weapon); // add to inventory
    equipment.weapon = entt::null;
}

// TODO: make work
void Drop(entt::registry& registry, entt::entity owner, size_t index, ItemCount_t amount) {}

}; // namespace ssg::inventory