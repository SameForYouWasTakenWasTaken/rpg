---@meta

---@class FloatRect
---@overload fun(): FloatRect
---@overload fun(x: number, y: number, w: number, h: number): FloatRect
---@field x number
---@field y number
---@field width number
---@field height number
FloatRect = {}

---@return number cx
---@return number cy
function FloatRect:center() end

---@param other FloatRect
---@return FloatRect?
function FloatRect:intersection(other) end
