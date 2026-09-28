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
---@field origin? Vec2

---@class TextureRef
---@field asset_handle TextureHandle
---@field float_rect FloatRect

---@class TextureInit
---@field asset_handle? TextureHandle
---@field float_rect? FloatRect

---@class InventoryRef
---@field capacity integer
---@field items Entity[]


---@class InventoryInit
---@field capacity? integer
---@field items? Entity[]

---@class Texture
---@field width integer
---@field height integer
---@field smooth boolean
---@field repeated boolean
---@field srgb boolean
---@field asset_handle TextureHandle
