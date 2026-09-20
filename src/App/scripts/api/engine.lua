---@meta

---@class engine
engine = {}

---@class engine.entity
engine.entity = {}

---Creates an entity in the active scene's registry.
---@return Entity
function engine.entity.create() end

---@class Entity
local Entity = {}

---@return boolean
function Entity:valid() end

---Destroys the entity. `valid()` returns false afterwards.
function Entity:destroy() end

---@param name "transform"
---@param init? TransformInit
---@overload fun(self: Entity, name: "sprite", init?: SpriteInit)
---@overload fun(self: Entity, name: "texture", init?: TextureInit)
function Entity:add(name, init) end

---@param name "transform"|"sprite"|"texture"
---@return boolean
function Entity:has(name) end

---@param name "transform"|"sprite"|"texture"
function Entity:remove(name) end

---Returns a live reference, or nil if the entity lacks the component.
---@param name "transform"
---@return TransformRef?
---@overload fun(self: Entity, name: "sprite"): SpriteRef?
---@overload fun(self: Entity, name: "texture"): TextureRef?
function Entity:get(name) end

---@class engine.assets
engine.assets = {}

---@param path string
---@return TextureHandle
function engine.assets.get_texture_handle(path) end

---@param handle TextureHandle
---@return Texture
function engine.assets.get_texture(handle) end
