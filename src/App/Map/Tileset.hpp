#pragma once
#include <utility>

#include "Rendering/Atlas.hpp"
#include "Types.hpp"

namespace ssg::map
{
class Tileset
{
  public:
    Tileset(const String& m_name, std::uint32_t m_tile_width, std::uint32_t m_tile_height,
            std::uint32_t m_columns, std::uint32_t m_tile_count, std::uint32_t m_firstgid,
            TextureHandle texture_id)
        : m_Name(m_name), m_tileWidth(m_tile_width), m_tileHeight(m_tile_height),
          m_Columns(m_columns), m_tileCount(m_tile_count), m_firstgid(m_firstgid),
          textureID(texture_id)
    {
    }

    [[nodiscard]] std::uint32_t getColumns() const { return m_Columns; }
    [[nodiscard]] String getName() const { return m_Name; }
    [[nodiscard]] std::uint32_t getWidth() const { return m_tileWidth; }
    [[nodiscard]] std::uint32_t getHeight() const { return m_tileHeight; }
    [[nodiscard]] std::uint32_t getTileCount() const { return m_tileCount; }
    [[nodiscard]] std::uint32_t getFirstGid() const { return m_firstgid; }
    [[nodiscard]] TextureHandle getTextureID() const { return textureID; }

    [[nodiscard]] Region getRegion(std::uint32_t localId) const;

  private:
    String m_Name;

    std::uint32_t m_tileWidth, m_tileHeight, m_Columns = 0;
    std::uint32_t m_tileCount = 0;
    std::uint32_t m_firstgid = 0;
    TextureHandle textureID = -1;
};
} // namespace ssg::map