---@meta

---@class Camera
---@field rotation Vec2
---@field size Vec2
---@field center Vec2
---@field zoom number
Camera = {}

---@return Camera
function Camera.new() end

---@overload fun(center: Vec2, size: Vec2): Camera
---@return Camera
function Camera.new(center, size) end

---@param amount Vec2
---@return nil
function Camera:move(amount) end

---@param amount number
---@return nil
function Camera:rotate(amount) end

---@param amount number
---@return nil
function Camera:zoom_by(amount) end
