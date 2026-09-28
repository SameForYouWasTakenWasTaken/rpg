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

---@param name "none"
---@param init? NoneInit
---@overload fun(self: Entity, name: "transform", init?: TransformRef)
---@overload fun(self: Entity, name: "sprite", init?: SpriteInit)
---@overload fun(self: Entity, name: "texture", init?: TextureInit)
---@overload fun(self: Entity, name: "inventory", init?: InventoryInit)
---@overload fun(self: Entity, name: "relationship", init?: NoneInit)
function Entity:add(name, init)
end

---@param name "none"|"transform"|"sprite"|"texture"|"inventory"|"relationship"
---@return boolean
function Entity:has(name)
end

---@param name "none"|"transform"|"sprite"|"texture"|"inventory"|"relationship"
function Entity:remove(name)
end

---Returns a live reference, or nil if the entity lacks the component.
---@param name "none"
---@return NoneRef?
---@overload fun(self: Entity, name: "transform"): TransformRef?
---@overload fun(self: Entity, name: "sprite"): SpriteRef?
---@overload fun(self: Entity, name: "texture"): TextureRef?
---@overload fun(self: Entity, name: "inventory"): InventoryRef?
---@overload fun(self: Entity, name: "relationship"): RelationshipRef?
function Entity:get(name)
end
