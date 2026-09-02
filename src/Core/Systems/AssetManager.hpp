#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Rendering/Atlas.hpp"
#include "Types.hpp"

namespace ssg
{
struct EntityDefinition
{
    std::string nameID{};

    AtlasID atlasID{};
    Region region{};

    bool IsValid() { return !nameID.empty(); }
};

struct AtlasConfig
{
    Filepath texture{};
    Filepath metadata{};
};

class AssetManager
{
  public:
    AssetManager() = default;
    ~AssetManager() = default;

    AssetManager(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    TextureID LoadTexture(const Filepath&);

    const sf::Texture& GetTexture(TextureID);
    const sf::Texture& GetTexture(const Filepath&);

    AtlasID LoadAtlas(EngineContext& context, AtlasConfig config);
    AtlasID LoadAtlas(
        EngineContext& context, const Filepath&,
        std::string_view field); // Load atlas from a .json field, which contains all configuration

    Atlas& GetAtlas(AtlasID);

    EntityDefinition GetEntityDefinition(const Filepath&);

  private:
    TextureID m_NextTextureID = 0;

    std::unordered_map<TextureID, std::unique_ptr<sf::Texture>> m_Textures = {};
    std::unordered_map<Filepath, TextureID> m_Filepaths = {};

    std::unordered_map<AtlasID, Atlas> m_Atlases;
};
} // namespace ssg