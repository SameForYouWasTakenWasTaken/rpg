---@meta

---@class engine.assets.map.Tilelayer
---@field name string
---@field width integer
---@field height integer
---@field tiles integer[]
local Tilelayer = {}


---Get the tile ID at the specified tile coordinates.
---@param x integer
---@param y integer
---@return integer
function Tilelayer:at(x, y) end
