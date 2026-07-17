#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Types.hpp"

namespace ssg
{
class AssetManager
{
  public:
    AssetManager() = default;
    ~AssetManager() = default;

    AssetManager(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    TextureID LoadTexture(Filepath);

    const sf::Texture& GetTexture(TextureID);
    const sf::Texture& GetTexture(Filepath);

  private:
    TextureID counter = 0;
    std::unordered_map<TextureID, std::unique_ptr<sf::Texture>> m_Textures = {};
    std::unordered_map<Filepath, TextureID> m_Filepaths = {};
};
} // namespace ssg