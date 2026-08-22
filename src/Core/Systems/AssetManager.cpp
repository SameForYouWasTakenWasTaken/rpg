#include "AssetManager.hpp"

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

#include "JsonUtil.hpp"

namespace ssg
{
TextureID AssetManager::LoadTexture(const Filepath& path)
{
    if (m_Filepaths.find(path) != m_Filepaths.end())
        return m_Filepaths[path]; // Texture is already loaded

    auto texture = std::make_unique<sf::Texture>(path);
    if (!texture)
        throw std::runtime_error("Could not get texture of path " + path.string() + "!");

    m_Textures.emplace(m_NextTextureID, std::move(texture));
    m_Filepaths.emplace(path, m_NextTextureID);

    return m_NextTextureID++;
}

const sf::Texture& AssetManager::GetTexture(TextureID id) { return *(m_Textures[id]); }
const sf::Texture& AssetManager::GetTexture(const Filepath& path)
{
    TextureID id = m_Filepaths[path];
    return GetTexture(id);
}
AtlasID AssetManager::LoadAtlas(AtlasConfig config)
{
    Atlas atlas;

    TextureID textureID = LoadTexture(config.texture);
    atlas.LoadAtlas(config.metadata, textureID);

    AtlasID id = atlas.GetID();
    m_Atlases.emplace(id, std::move(atlas));

    return id;
}

AtlasID AssetManager::LoadAtlas(const Filepath& jsonPath, std::string_view field)
{
    Atlas atlas;

    json::json data;
    std::ifstream file(jsonPath);
    if (!file.is_open())
        throw std::runtime_error("Could not open atlas .json file: " + jsonPath.string());

    file >> data;

    auto atlasConfig = json::AccessObjectField(data, field);
    auto texture = json::AttemptAccessField<Filepath>(atlasConfig, "texture");
    auto jsonMetadata = json::AttemptAccessField<Filepath>(atlasConfig, "metadata");

    TextureID textureID = LoadTexture(texture);
    atlas.LoadAtlas(jsonMetadata, textureID);

    AtlasID id = atlas.GetID();
    m_Atlases.emplace(id, std::move(atlas));

    return id;
}

Atlas& AssetManager::GetAtlas(AtlasID id)
{
    auto it = m_Atlases.find(id);

    if (it == m_Atlases.end())
        throw std::runtime_error("Couldn't find loaded atlas with ID: " + id);

    return it->second;
}

EntityDefinition AssetManager::GetEntityDefinition(const Filepath& filepath)
{
    EntityDefinition definition{};

    json::json data;

    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Could not open entity .json file: " + filepath.string());

    file >> data;

    if (!ssg::json::Has(data, "id") || !ssg::json::Has(data, "components"))
    {
        return EntityDefinition{};
    }

    const auto& components = ssg::json::AccessObjectField(data, "components");

    if (!ssg::json::Has(components, "sprite"))
        return EntityDefinition{};

    const auto& spriteField = ssg::json::AccessObjectField(components, "sprite");

    definition.nameID = ssg::json::AttemptAccessField<String>(data, "id");

    definition.atlasID = ssg::json::AttemptAccessField<String>(spriteField, "atlas");

    const String regionName = ssg::json::AttemptAccessField<String>(spriteField, "region");

    definition.region = GetAtlas(definition.atlasID).GetRegion(regionName);

    return definition;
}

} // namespace ssg