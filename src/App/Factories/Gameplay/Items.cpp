#include "Items.hpp"

#include "Components/Gameplay/Inventory/CItem.hpp"
#include "Factories/Default.hpp"

namespace ssg::factory
{
json::json ApplyItemDefinition(entt::registry& r, entt::entity entity, Filepath definition)
{
    auto data = ApplyCharacterDefinition(r, entity, definition);
    ItemCount_t maxCount = json::AttemptAccessField<ItemCount_t>(data, "maxCount");

    CItem item;
    item.itemTypeId = json::AttemptAccessField<std::string>(data, "id");
    item.maxStack = maxCount;

    r.emplace_or_replace<CItem>(entity, std::move(item));

    return data;
}
} // namespace ssg::factory