#pragma once

#include "../../../Map/Tilemap.hpp"
#include "EngineContext.hpp"
#include "JsonUtil.hpp"

namespace ssg::map
{
namespace Tiled
{
Tilemap LoadTilemapJSON(EngineContext& context, const Filepath& path);
TileLayer LoadTileLayerJSON(const Filepath& path, const json::json& layerJson);
Tileset LoadTilesetXML(EngineContext& context, const Filepath& path, uint32_t firstgid);
} // namespace Tiled

} // namespace ssg::map