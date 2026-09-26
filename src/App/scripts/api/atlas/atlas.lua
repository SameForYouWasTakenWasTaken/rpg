---@meta

---@class engine.assets.Atlas
---@field id string
---@field asset_handle TextureHandle
engine.assets.Atlas = {}

---@param name string
---@return Region
function engine.assets.Atlas:get_region(name) end

---@return table<string, Region>
function engine.assets.Atlas:get_regions() end
