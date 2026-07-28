#include "Default.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Components/CDefinition.hpp"
#include "Components/CRelationship.hpp"
#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"
#include "Components/CTransform.hpp"
#include "Components/CWorldTransform.hpp"
#include "Engine.hpp"

template <typename TExpected>
static TExpected AttemptAccessField(const nlohmann::json& data, std::string_view field)
{
    auto it = data.find(field);
    if (it == data.end())
    {
        std::string id = data.value("id", "UNKNOWN_ENTITY");
        throw std::runtime_error("Missing required field in JSON for entity");
    }

    try
    {
        return it->get<TExpected>();
    }
    catch (const nlohmann::json::exception& e)
    {
        // Catch type mismatch errors (e.g., field exists, but tried to read a string as int)
        throw std::runtime_error(e.what());
    }
}

static const nlohmann::json& AccessObjectField(const nlohmann::json& data, std::string_view field)
{
    auto it = data.find(field);
    if (it == data.end())
        throw std::runtime_error("Could not access field");

    return *it;
}

static ssg::Vec2 ReadVec2(const nlohmann::json& data, std::string_view field,
                          std::string_view x = "x", std::string_view y = "y")
{
    const auto& object = AccessObjectField(data, field);

    return {AttemptAccessField<float>(object, x), AttemptAccessField<float>(object, y)};
}

static ssg::Vec3 ReadVec3(const nlohmann::json& data, std::string_view field,
                          std::string_view x = "x", std::string_view y = "y",
                          std::string_view z = "z")
{
    const auto& object = AccessObjectField(data, field);

    return {AttemptAccessField<float>(object, x), AttemptAccessField<float>(object, y),
            AttemptAccessField<float>(object, z)};
}

static ssg::Vec4 ReadVec4(const nlohmann::json& data, std::string_view field,
                          std::string_view x = "x", std::string_view y = "y",
                          std::string_view z = "z", std::string_view w = "w")
{
    const auto& object = AccessObjectField(data, field);

    return {AttemptAccessField<float>(object, x), AttemptAccessField<float>(object, y),
            AttemptAccessField<float>(object, z), AttemptAccessField<float>(object, w)};
}
namespace ssg::factory
{
void ApplyCharacterDefinition(entt::registry& r, entt::entity entity, Filepath definition)
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
        auto atlasID = AttemptAccessField<AtlasID>(component, "atlas");
        auto region = AttemptAccessField<String>(component, "region");

        auto zIndex = AttemptAccessField<zIndex_t>(component, "z-index");

        auto origin = ReadVec2(component, "origin");
        auto size = ReadVec2(component, "size");
        auto color = ReadVec4(component, "color", "r", "g", "b", "a");

        auto flipX = AttemptAccessField<bool>(component, "flip-x");
        auto flipY = AttemptAccessField<bool>(component, "flip-y");

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

    if (components.contains("transform"))
    {
        const auto& component = components.at("transform");
        auto position = ReadVec2(component, "position");

        auto scale = ReadVec2(component, "scale");
        auto rotation = AttemptAccessField<float>(component, "rotation");

        CTransform transform;
        transform.position = Vec2{position.x, position.y};
        transform.scale = Vec2{scale.x, scale.y};
        transform.rotation = rotation;
        r.emplace_or_replace<CTransform>(entity, std::move(transform));
        // Add additional world transform for hierarchial entities
        auto& worldTransform = r.emplace_or_replace<CWorldTransform>(entity);
    }

    auto& definitionComponent = r.emplace_or_replace<CDefinition>(entity);
    definitionComponent.filepath = definition;
}
} // namespace ssg::factory
