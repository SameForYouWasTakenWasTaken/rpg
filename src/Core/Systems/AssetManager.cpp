#include "AssetManager.hpp"

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

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
AtlasID AssetManager::LoadAtlas(const Filepath& jsonPath, const Filepath& texturePath)
{
    Atlas atlas;

    TextureID textureID = LoadTexture(texturePath);
    atlas.LoadAtlas(jsonPath, textureID);

    AtlasID id = atlas.GetID();
    m_Atlases.emplace(id, std::move(atlas));

    return id;
}

Atlas& AssetManager::GetAtlas(AtlasID id) { return m_Atlases.at(id); }

EntityDefinition AssetManager::GetEntityDefinition(const Filepath& filepath)
{
    using namespace nlohmann;
    EntityDefinition definition{};

    json data;
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Could not open atlas .json file: " + filepath.string());

    file >> data;

    if (!data.contains("id") || !data.contains("sprite"))
        return EntityDefinition{};

    auto& spriteField = data.at("sprite");
    auto regionName = spriteField.at("region").get<String>();

    definition.nameID = data.at("id").get<String>();
    definition.atlasID = spriteField.at("atlas").get<String>();

    Region region = GetAtlas(definition.atlasID).GetRegion(regionName);
    definition.region = region;

    return definition;
}

} // namespace ssg