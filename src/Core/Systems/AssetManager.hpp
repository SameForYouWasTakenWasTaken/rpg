#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "EngineContext.hpp"
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

    TextureHandle LoadTexture(const Filepath&);

    const sf::Texture& GetTexture(TextureHandle);
    const sf::Texture& GetTexture(const Filepath&);

    void LoadAtlas(const Atlas& atlas);

    Atlas& GetAtlas(const AtlasID&);

    EntityDefinition GetEntityDefinition(const Filepath&);

  private:
    TextureHandle::value_type m_NextTextureID = 0;

    std::unordered_map<TextureHandle::value_type, std::unique_ptr<sf::Texture>> m_Textures;
    std::unordered_map<Filepath, TextureHandle> m_Filepaths = {};

    std::unordered_map<AtlasID, Atlas> m_Atlases;
};
} // namespace ssg