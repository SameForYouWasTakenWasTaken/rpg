---@meta

---@class engine.hierarchy
engine.hierarchy = {}

---@param parent Entity
---@param child Entity
function engine.hierarchy.attach_child(parent, child) end

---@param child Entity
function engine.hierarchy.detach_child(parent, child) end

---@param entity Entity
---@return boolean
function engine.hierarchy.is_root(entity) end

---@param entity Entity
---@return Entity?
function engine.hierarchy.get_root(entity) end
