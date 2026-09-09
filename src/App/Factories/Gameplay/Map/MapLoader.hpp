#pragma once

#include "Config/Gameplay/Map/MapConfig.hpp"
#include "EngineContext.hpp"
#include "JsonUtil.hpp"
#include "Map/Tilemap.hpp"

namespace ssg::map
{
struct MapEntry
{
    Filepath mapConfigPath{};
};

MapEntry LookUpMapEntry(const String& field,
                        const Filepath& filepath = Config::map::DEFAULT_MAPS_PATH);
namespace Tiled
{
Tilemap LoadTilemapJSON(EngineContext& context, const Filepath& path);
TileLayer LoadTileLayerJSON(const Filepath& path, const json::json& layerJson);
Tileset LoadTilesetXML(EngineContext& context, const Filepath& path, uint32_t firstgid);
} // namespace Tiled
} // namespace ssg::map