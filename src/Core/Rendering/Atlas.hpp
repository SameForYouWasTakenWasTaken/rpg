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
    Region(Vec2 position = {}, Vec2 size = {}) : position(position), size(size) {}

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

    Atlas(TextureHandle textureID, AtlasID id, RegionList regions)
        : m_TextureID(textureID), m_ID(std::move(id)), m_Regions(std::move(regions))
    {
    }

    const AtlasID& GetID() const { return m_ID; }
    TextureHandle GetTextureID() const { return m_TextureID; }
    std::optional<Region> GetRegion(const String& name) const;

    const RegionList& GetAllRegions() const { return m_Regions; }

  private:
    AtlasID m_ID;
    TextureHandle m_TextureID;
    RegionList m_Regions;
};

inline std::optional<Region> Atlas::GetRegion(const String& name) const
{
    auto it = m_Regions.find(name);

    if (it == m_Regions.end())
        return std::nullopt;

    return it->second;
}
} // namespace ssg