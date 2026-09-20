---@meta

---2D float vector (glm::vec2).
---@class Vec2
---@overload fun(): Vec2
---@overload fun(s: number): Vec2
---@overload fun(x: number, y: number): Vec2
---@field x number
---@field y number
---@operator add(Vec2): Vec2
---@operator sub(Vec2): Vec2
---@operator mul(number): Vec2
---@operator mul(Vec2): Vec2 # component-wise
---@operator div(number): Vec2
---@operator unm: Vec2
Vec2 = {}

---@return number
function Vec2:length() end

---@return number
function Vec2:length_squared() end

---Returns a zero vector if the length is 0 (no NaN).
---@return Vec2
function Vec2:normalized() end

---@param other Vec2
---@return number
function Vec2:dot(other) end

---@param other Vec2
---@return number
function Vec2:distance(other) end

---@param other Vec2
---@param t number 0..1
---@return Vec2
function Vec2:lerp(other, t) end

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
