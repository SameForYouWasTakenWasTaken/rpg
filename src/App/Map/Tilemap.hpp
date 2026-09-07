#pragma once
#include <cstdint>
#include <optional>

#include "ObjectLayer.hpp"
#include "TileLayer.hpp"
#include "Tileset.hpp"
#include "Types.hpp"

namespace ssg::map
{
class Tilemap
{
  public:
    Tilemap(std::uint32_t width, std::uint32_t height, std::uint32_t m_tile_height,
            std::uint32_t m_tile_width, const Vector<TileLayer>& m_layers,
            const Vector<Tileset>& m_sets, const Vector<ObjectLayer>& m_object_layers)
        : width(width), height(height), m_TileHeight(m_tile_height), m_TileWidth(m_tile_width),
          m_Layers(m_layers), m_Sets(m_sets), m_ObjectLayers(m_object_layers)
    {
    }

    [[nodiscard]] std::uint32_t getWidth() const { return width; }
    [[nodiscard]] std::uint32_t getHeight() const { return height; }
    [[nodiscard]] std::uint32_t getTileHeight() const { return m_TileHeight; }
    [[nodiscard]] std::uint32_t getTileWidth() const { return m_TileWidth; }

    [[nodiscard]] const Vector<TileLayer>& getTileLayers() const { return m_Layers; }
    [[nodiscard]] const Vector<Tileset>& getTilesets() const { return m_Sets; }
    [[nodiscard]] const Vector<ObjectLayer>& getObjectLayers() const { return m_ObjectLayers; }

    [[nodiscard]] const Tileset* getTilesetForGid(TileLayer::TileID gid) const;

  private:
    std::uint32_t width, height = 0;
    std::uint32_t m_TileHeight = 0, m_TileWidth = 0;

    Vector<TileLayer> m_Layers;
    Vector<Tileset> m_Sets;
    Vector<ObjectLayer> m_ObjectLayers;
};
} // namespace ssg::map