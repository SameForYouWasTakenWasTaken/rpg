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
void AssetManager::LoadAtlas(Atlas atlas)
{
    auto it = m_Atlases.find(atlas.GetID());
    if (it != m_Atlases.end())
        return;

    m_Atlases.emplace(atlas.GetID(), atlas);
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

    if (!json::Has(data, "id") || !json::Has(data, "components"))
    {
        return EntityDefinition{};
    }

    const auto& components = json::AccessObjectField(data, "components");

    if (!json::Has(components, "sprite"))
        return EntityDefinition{};

    const auto& spriteField = json::AccessObjectField(components, "sprite");

    definition.nameID = json::AttemptAccessField<String>(data, "id");

    definition.atlasID = json::AttemptAccessField<String>(spriteField, "atlas");

    const String regionName = json::AttemptAccessField<String>(spriteField, "region");

    definition.region = GetAtlas(definition.atlasID).GetRegion(regionName);

    return definition;
}

} // namespace ssg