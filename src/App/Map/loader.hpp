#pragma once

#include "EngineContext.hpp"
#include "JsonUtil.hpp"
#include "api/Tilemap.hpp"

namespace ssg::map
{
namespace Tiled
{
Tilemap LoadTilemap(EngineContext& context, const Filepath& path);
TileLayer LoadTileLayer(const Filepath& path, const json::json& layerJson);
Tileset LoadTileset(EngineContext& context, const Filepath& path, uint32_t firstgid);
} // namespace Tiled

} // namespace ssg::map