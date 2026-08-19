#include "Default.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Components/CDefinition.hpp"
#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"
#include "Components/CTransform.hpp"
#include "Components/CWorldTransform.hpp"
#include "Components/Gameplay/CCombatState.hpp"
#include "Components/Gameplay/CEquipment.hpp"
#include "Components/Gameplay/CHealth.hpp"
#include "Components/Gameplay/CHumanoid.hpp"
#include "Components/Gameplay/Inventory/CInventory.hpp"
#include "Engine.hpp"
#include "JsonUtil.hpp"

namespace ssg::factory
{

json::json ApplyCharacterDefinition(entt::registry& r, entt::entity entity, Filepath definition)
{
    using namespace nlohmann;
    auto& assetManager = Engine::instance().assetManager;

    json::json data;
    std::ifstream file(definition);

    if (!file.is_open())
        throw std::runtime_error("Could not open file " + definition.string() + "!");

    file >> data;
    if (!data.contains("id") || !data.contains("components"))
        throw std::runtime_error("Definition data does not contain character data: " +
                                 definition.string());

    auto& components = data.at("components");
    if (json::Has(components, "sprite"))
    {
        const auto& component = components.at("sprite");
        auto atlasID = json::AttemptAccessField<AtlasID>(component, "atlas");
        auto region = json::AttemptAccessField<String>(component, "region");

        auto zIndex = json::AttemptAccessField<zIndex_t>(component, "z-index");

        auto origin = json::ReadVec2(component, "origin");
        auto size = json::ReadVec2(component, "size");
        auto color = json::ReadVec4(component, "color", "r", "g", "b", "a");

        auto flipX = json::AttemptAccessField<bool>(component, "flip-x");
        auto flipY = json::AttemptAccessField<bool>(component, "flip-y");

        CSprite sprite;
        sprite.color = sf::Color(color.r, color.g, color.b, color.a);
        sprite.flipX = flipX;
        sprite.flipY = flipY;
        sprite.origin = Vec2{origin.x, origin.y};
        sprite.size = Vec2{size.x, size.y};
        sprite.zIndex = zIndex;

        r.emplace_or_replace<CSprite>(entity, std::move(sprite));

        auto& atlas = assetManager.GetAtlas(atlasID);
        auto regionRect = atlas.GetRegion(region);

        CTexture texture;
        texture.textureID = atlas.GetTextureID();
        texture.textureRect = regionRect;

        r.emplace_or_replace<CTexture>(entity, std::move(texture));
    }

    if (json::Has(components, "transform"))
    {
        const auto& component = components.at("transform");
        auto position = json::ReadVec2(component, "position");

        auto scale = json::ReadVec2(component, "scale");
        auto rotation = json::AttemptAccessField<float>(component, "rotation");

        CTransform transform;
        transform.position = Vec2{position.x, position.y};
        transform.scale = Vec2{scale.x, scale.y};
        transform.rotation = rotation;
        r.emplace_or_replace<CTransform>(entity, std::move(transform));
        // Add additional world transform for hierarchial entities
        auto& worldTransform = r.emplace_or_replace<CWorldTransform>(entity);
    }

    if (json::Has(components, "health"))
    {
        const auto& component = json::AccessObjectField(components, "health");
        auto max = json::AttemptAccessField<float>(component, "max");
        auto current = json::AttemptAccessField<float>(component, "current");

        CHealth health;
        health.max = max;
        health.current = current;
        r.emplace_or_replace<CHealth>(entity, std::move(health));
    }

    if (json::Has(components, "humanoid"))
    {
        const auto& component = json::AccessObjectField(components, "humanoid");
        auto speed = json::AttemptAccessField<float>(component, "speed");

        CHumanoid humanoid;
        humanoid.speed = speed;
        r.emplace_or_replace<CHumanoid>(entity, std::move(humanoid));
    }

    // Managed by systems
    if (json::Has(components, "combatState"))
    {
        CCombatState state;
        r.emplace_or_replace<CCombatState>(entity, std::move(state));
    }

    // Managed by systems
    if (json::Has(components, "equipment"))
    {
        CEquipment equipment;
        r.emplace_or_replace<CEquipment>(entity, std::move(equipment));
    }

    if (json::Has(components, "inventory"))
    {
        const auto& component = json::AccessObjectField(components, "inventory");
        CInventory inventory;
        auto capacity = json::AttemptAccessField<size_t>(component, "capacity");
        inventory.capacity = capacity;
        r.emplace_or_replace<CInventory>(entity, std::move(inventory));
    }

    auto& definitionComponent = r.emplace_or_replace<CDefinition>(entity);
    definitionComponent.filepath = definition;

    return data;
}
} // namespace ssg::factory
