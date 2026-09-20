---@meta

---@class TransformRef
---@field position Vec2
---@field rotation number
---@field scale Vec2

---@class TransformInit
---@field position? Vec2
---@field rotation? number
---@field scale? Vec2

---@class SpriteRef
---@field color any
---@field z_index integer
---@field origin Vec2
---@field size Vec2
---@field flip_x boolean
---@field flip_y boolean

---@class SpriteInit
---@field size? Vec2
---@field z_index? integer
---@field flip_x? boolean
---@field flip_y? boolean

---@class TextureRef
---@field texture_handle TextureHandle
---@field float_rect FloatRect

---@class TextureInit
---@field texture_handle? TextureHandle
---@field float_rect? FloatRect

---@class TextureHandle
---@field id integer
local TextureHandle = {}
---@return boolean
function TextureHandle:valid() end

---@class Texture
---@field width integer
---@field height integer
---@field smooth boolean
---@field repeated boolean
---@field srgb boolean
