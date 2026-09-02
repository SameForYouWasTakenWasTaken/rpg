#pragma once

#include <unordered_map>

#include "SFML/Graphics/Rect.hpp"
#include "Types.hpp"

namespace ssg
{
struct EngineContext;
using Region = sf::FloatRect;
using RegionList = std::unordered_map<String, Region>;
class Atlas
{
  public:
    Atlas(EngineContext& engineContext) : m_EngineContext(engineContext) {}
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

    EngineContext& m_EngineContext;
};
} // namespace ssg