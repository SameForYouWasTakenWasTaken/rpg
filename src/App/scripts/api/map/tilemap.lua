---@meta

---@class engine.assets.map.Tilemap
---@field width integer Width of the map in tiles.
---@field height integer Height of the map in tiles.
---@field tile_width integer Width of each tile in pixels.
---@field tile_height integer Height of each tile in pixels.
---@field tile_layers engine.assets.map.Tilelayer[] Tile layers contained in the map.
---@field tilesets engine.assets.map.Tileset[] Tilesets used by the map.
local Tilemap = {}

---Get the tileset responsible for a global tile ID.
---
---@param gid integer Global tile ID.
---@return engine.assets.map.Tileset? tileset
function Tilemap:get_tileset_for_gid(gid)
end

---Create the entities represented by this map
---in the current scene.
function Tilemap:create()
end
