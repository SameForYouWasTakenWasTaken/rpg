#pragma once

#include <unordered_map>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Types.hpp"

namespace ssg
{
using Region = sf::FloatRect;
using RegionList = std::unordered_map<String, Region>;
class Atlas
{
  public:
    Atlas() = default;
    ~Atlas() = default;

    // Atlas(const Atlas&) = delete;
    // Atlas(Atlas&&) = delete;

    // Atlas& operator=(const Atlas&) = delete;
    // Atlas& operator=(Atlas&&) = delete;

    TextureID LoadAtlas(const Filepath&, const Filepath&);
    void LoadAtlas(const Filepath&, TextureID);
    sf::FloatRect GetRegion(const String&);
    const RegionList& GetAllRegions();

    const std::string& GetID() const { return m_ID; }
    TextureID GetTextureID() { return m_TextureID; }

  private:
    std::string m_ID{};
    TextureID m_TextureID{};
    Filepath m_JsonFilepath = "";

    // key: subtexture filename (such as player_walk.jpg)
    // sf::FloatRect: the texture dimensions
    RegionList m_Regions = {};
};
} // namespace ssg