#include "Weapons.hpp"

#include "Components/Gameplay/CWeapon.hpp"
#include "Items.hpp"
#include "JsonUtil.hpp"
#include "Logger.hpp"

namespace ssg::factory
{
json::json ApplyWeaponDefinition(EngineContext& context, entt::registry& r, entt::entity entity,
                                 Filepath definition)
{
    json::json data =
        ApplyItemDefinition(context, r, entity, definition); // Adds sprites, textures, etc
    if (data.is_null())
        return data;

    CWeapon weapon;
    weapon.name = json::AttemptAccessField<std::string>(data, "id");
    weapon.damage = json::AttemptAccessField<float>(data, "damage");
    weapon.range = json::AttemptAccessField<float>(data, "range");
    weapon.hitWindow = json::ReadVec2(data, "hitWindow", "min", "max");
    weapon.attackSpeed = json::AttemptAccessField<float>(data, "attackSpeed");

    if (weapon.hitWindow.x < 0 || weapon.hitWindow.y < 0 ||
        weapon.hitWindow.x > weapon.hitWindow.y || weapon.damage < 0 || weapon.range < 0 ||
        weapon.attackSpeed < 0)
    {
        context.logger.Fatal(
            "Factory",
            "Error occured during weapon creation: Invalid weapon data!\n\n ID: {}, "
            "Hitwindow: {}, {}, Damage: {}, Range: {}, Speed: {}",
            weapon.name, weapon.hitWindow.x, weapon.hitWindow.y, weapon.damage, weapon.range,
            weapon.attackSpeed);
        return json::json{nullptr};
    }

    r.emplace<CWeapon>(entity, std::move(weapon));
    return data;
}
} // namespace ssg::factory