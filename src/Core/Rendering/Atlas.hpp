#pragma once

#include <unordered_map>

#include "SFML/Graphics/Rect.hpp"
#include "Types.hpp"

namespace ssg
{
struct Region
{
    Vec2 position;
    Vec2 size;
    Region(Vec2 position = {}, Vec2 size = {}) : position(position), size(size) {};

    operator sf::FloatRect() const
    {
        return sf::FloatRect{{position.x, position.y}, {size.x, size.y}};
    }
};

using RegionList = std::unordered_map<String, Region>;

class Atlas
{
  public:
    Atlas() = default;

    Atlas(TextureID textureID, std::string id, RegionList regions)
        : m_TextureID(textureID), m_ID(std::move(id)), m_Regions(std::move(regions))
    {
    }

    const std::string& GetID() const { return m_ID; }
    TextureID GetTextureID() const { return m_TextureID; }

    const Region& GetRegion(const String& name) const { return m_Regions.at(name); }

    const RegionList& GetAllRegions() const { return m_Regions; }

  private:
    std::string m_ID;
    TextureID m_TextureID;
    RegionList m_Regions;
};
} // namespace ssg