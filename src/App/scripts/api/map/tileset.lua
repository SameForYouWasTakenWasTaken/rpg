---@meta

---@class engine.assets.map.Tileset
---@field columns integer Number of tile columns in the tileset.
---@field name string Name of the tileset.
---@field width integer Width of each tile in pixels.
---@field height integer Height of each tile in pixels.
---@field tile_count integer Number of tiles in the tileset.
---@field first_gid integer First global tile ID belonging to this tileset.
---@field asset_handle TextureHandle Texture asset handle used by the tileset.
local Tileset = {}

---Get the texture region for a local tile ID.
---@param local_id integer Local tile ID within the tileset.
---@return Region
function Tileset:get_region(local_id)
end
