#include "Default.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Components/CRelationship.hpp"
#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"
#include "Components/CTransform.hpp"
#include "Components/CWorldTransform.hpp"
#include "Engine.hpp"

template <typename TExpected>
static inline TExpected AttemptAccessCharacterField(const nlohmann::json& data,
                                                    const std::string& field)
{
    auto it = data.find(field);
    if (it == data.end())
    {
        std::string id = data.value("id", "UNKNOWN_ENTITY");
        throw std::runtime_error("Missing required field '" + field +
                                 "' in JSON for entity ID: " + id);
    }

    try
    {
        return it->get<TExpected>();
    }
    catch (const nlohmann::json::exception& e)
    {
        // Catch type mismatch errors (e.g., field exists, but tried to read a string as int)
        throw std::runtime_error("Type mismatch for field '" + field + "': " + e.what());
    }
}

namespace ssg::factory
{
void CreateCharacter(entt::registry& r, entt::entity entity, const Filepath& definition)
{
    using namespace nlohmann;
    auto& assetManager = Engine::instance().assetManager;

    json data;
    std::ifstream file(definition);

    if (!file.is_open())
        throw std::runtime_error("Could not open file " + definition.string() + "!");

    file >> data;
    if (!data.contains("id") || !data.contains("components"))
        throw std::runtime_error("Definition data does not contain character data: " +
                                 definition.string());

    auto& components = data.at("components");
    if (components.contains("sprite"))
    {
        const auto& component = components.at("sprite");
        auto atlasID = AttemptAccessCharacterField<AtlasID>(component, "atlas");
        auto region = AttemptAccessCharacterField<String>(component, "region");

        auto zIndex = AttemptAccessCharacterField<zIndex_t>(component, "z-index");

        auto originField = AttemptAccessCharacterField<json>(component, "origin");
        auto originX = AttemptAccessCharacterField<float>(originField, "x");
        auto originY = AttemptAccessCharacterField<float>(originField, "y");

        auto sizeField = AttemptAccessCharacterField<json>(component, "size");
        auto sizeX = AttemptAccessCharacterField<float>(sizeField, "x");
        auto sizeY = AttemptAccessCharacterField<float>(sizeField, "y");

        auto flipX = AttemptAccessCharacterField<bool>(component, "flip-x");
        auto flipY = AttemptAccessCharacterField<bool>(component, "flip-y");

        auto colorField = AttemptAccessCharacterField<json>(component, "color");
        auto colorR = AttemptAccessCharacterField<std::uint8_t>(colorField, "r");
        auto colorG = AttemptAccessCharacterField<std::uint8_t>(colorField, "g");
        auto colorB = AttemptAccessCharacterField<std::uint8_t>(colorField, "b");
        auto colorA = AttemptAccessCharacterField<std::uint8_t>(colorField, "a");

        auto& sprite = r.emplace_or_replace<CSprite>(entity);
        sprite.color = sf::Color(colorR, colorG, colorB, colorA);
        sprite.flipX = flipX;
        sprite.flipY = flipY;
        sprite.origin = Vec2{originX, originY};
        sprite.size = Vec2{sizeX, sizeY};
        sprite.zIndex = zIndex;

        auto& atlas = assetManager.GetAtlas(atlasID);
        auto regionRect = atlas.GetRegion(region);

        auto& texture = r.emplace_or_replace<CTexture>(entity);
        texture.textureID = atlas.GetTextureID();
        texture.textureRect = regionRect;
    }

    if (components.contains("transform"))
    {
        const auto& component = components.at("transform");
        auto positionField = AttemptAccessCharacterField<json>(component, "position");
        auto positionX = AttemptAccessCharacterField<float>(positionField, "x");
        auto positionY = AttemptAccessCharacterField<float>(positionField, "y");

        auto scaleField = AttemptAccessCharacterField<json>(component, "scale");
        auto scaleX = AttemptAccessCharacterField<float>(scaleField, "x");
        auto scaleY = AttemptAccessCharacterField<float>(scaleField, "y");

        auto rotation = AttemptAccessCharacterField<float>(component, "rotation");

        auto& transform = r.emplace_or_replace<CTransform>(entity);
        transform.position = Vec2{positionX, positionY};
        transform.scale = Vec2{scaleX, scaleY};
        transform.rotation = rotation;

        // Add additional world transform for hierarchial entities
        auto& worldTransform = r.emplace_or_replace<CWorldTransform>(entity);
    }
}
} // namespace ssg::factory
