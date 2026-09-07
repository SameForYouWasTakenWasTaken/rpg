#pragma once
#include <cstdint>
#include <utility>

#include "Types.hpp"

namespace ssg::map
{
class TileLayer
{
  public:
    using TileID = std::uint32_t;
    TileLayer(String name, std::uint32_t m_width, std::uint32_t m_height,
              const Vector<TileID>& m_tiles)
        : name(std::move(name)), m_Width(m_width), m_Height(m_height), m_Tiles(m_tiles)
    {
    }

    [[nodiscard]] String getName() const { return name; }
    [[nodiscard]] std::uint32_t getWidth() const { return m_Width; }
    [[nodiscard]] std::uint32_t getHeight() const { return m_Height; }
    [[nodiscard]] const Vector<TileID>& getTiles() const { return m_Tiles; }
    [[nodiscard]] TileID at(std::uint32_t x, std::uint32_t y) const;

  private:
    String name;
    std::uint32_t m_Width, m_Height = 0;

    Vector<TileID> m_Tiles;
};
} // namespace ssg::map