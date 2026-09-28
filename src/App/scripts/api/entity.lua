---@meta

---@class engine.entity
engine.entity = {}

---Creates an entity in the active scene's registry.
---@return Entity
function engine.entity.create()
end

-- Get all entities of a certain component
---@param ... string
---@return Entity[]
function engine.entity.get_all_of(...)
end

--============================= ENTITY CLASS =============================

---@class Entity
local Entity = {}

---@return boolean
function Entity:valid()
end

---Destroys the entity. `valid()` returns false afterwards.
function Entity:destroy()
end

---@param name "transform"
---@param init? TransformInit
---@overload fun(self: Entity, name: "sprite", init?: SpriteInit)
---@overload fun(self: Entity, name: "texture", init?: TextureInit)
---@overload fun(self: Entity, name: "inventory", init?: InventoryInit)
function Entity:add(name, init)
end

---@param name "transform"|"sprite"|"texture"|"inventory"
---@return boolean
function Entity:has(name)
end

---@param name "transform"|"sprite"|"texture"|"inventory"
function Entity:remove(name)
end

---Returns a live reference, or nil if the entity lacks the component.
---@param name "transform"
---@return TransformRef?
---@overload fun(self: Entity, name: "sprite"): SpriteRef?
---@overload fun(self: Entity, name: "texture"): TextureRef?
---@overload fun(self: Entity, name: "inventory"): InventoryRef?
function Entity:get(name)
end
